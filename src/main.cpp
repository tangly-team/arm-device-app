#include <iostream>

#include "vinci-cpp/inc/ActorCpp.h"
#include "vinci-cpp/inc/MsgPoolCpp.h"

#include "vinci-cmsis/inc/ActorCmsis.h"
#include "vinci-cmsis/inc/MsgPoolCmsis.h"
#include "vinci/inc/TimerMgr.h"

using namespace vinci;

struct Data;

struct ActorData {
    static const int ACTOR_COMMUNICATION = 1;
    static const int ACTOR_CONFIGURATION = 2;
    int cmd;
    int msgId;
    char* payload;
    Actor<Data>* sender;
    Actor<Data>* receiver;
};

typedef std::variant<ActorData, TimerCmd<Data>, Timer<Data>> Variants;

struct Data {
    Variants data;
};

typedef Message<Data> MyMessage;

MsgPoolCpp<Data> msgPoolCpp;
MsgPoolCmsis<Data> msgPoolCmsis;

class MyActorLogic {
public:
    MyActorLogic(MessagePool<Data>& pool) : _pool{pool} {}

    bool processMessage(Actor<Data>* actor, Message<Data>* msg);

    Message<Data>* build(ActorData data);

private:
    MessagePool<Data>& _pool;
    char payload[64] = "this is a payload";
};

Message<Data>* MyActorLogic::build(ActorData data) {
    Data dataVariant{data};
    Message<Data>* msg = _pool.acquire();
    msg->kind(dataVariant.data.index());
    msg->data(dataVariant);
    return msg;
}

bool MyActorLogic::processMessage(Actor<Data>* actor, Message<Data>* msg) {
    bool continues = true;
    Variants variants = msg->data().data;
    if (const ActorData* data = std::get_if<ActorData>(&variants)) {
        switch (data->cmd) {
            case ActorData::ACTOR_COMMUNICATION:
                std::cout << "received message " << data->msgId << " from " << data->sender->name() << " to " << data->receiver->name()
                          << "[[" << payload << "]]" << "{" << msg << ":" << msgPoolCpp.nrOfFreeMsgs() << "}" << std::endl;
                continues = (data->msgId < 20);
                Actor<Data>::send(*data->sender, build({ActorData::ACTOR_COMMUNICATION, data->msgId + 1, payload, data->receiver, data->sender}));
                break;
            case ActorData::ACTOR_CONFIGURATION:
                strcpy(payload, data->payload);
                Actor<Data>::send(*data->receiver, build({ActorData::ACTOR_CONFIGURATION, data->msgId + 1, payload, data->sender, data->receiver}));
                continues = (actor != data->sender);
                break;
        }
    } else if (const Timer<Data>* timer = std::get_if<Timer<Data>>(&variants)) {
        std::cout << timer->client()->name() << " " << timer->id() << std::endl;
    }
    _pool.release(msg);
    return continues;
}

class MyActorCpp : public ActorCpp<Data> {
public:
    MyActorCpp(const char* name, MsgPoolCpp<Data>& pool);

protected:
    bool processMsg(Message<Data>* msg);

private:
    MyActorLogic logic;
};

MyActorCpp::MyActorCpp(const char* name, MsgPoolCpp<Data>& pool) : ActorCpp<Data>(name, pool), logic(pool) {}

bool MyActorCpp::processMsg(Message<Data>* msg) {
    return logic.processMessage(this, msg);
};

class MyActorCmsis : public ActorCmsis<Data> {
public:
    MyActorCmsis(const char* name, int queueSize, MsgPoolCmsis<Data>& pool);

protected:
    bool processMsg(Message<Data>* msg);

private:
    MyActorLogic logic;
};

osThreadAttr_t threadAttributes;

MyActorCmsis::MyActorCmsis(const char* name, int queueSize, MsgPoolCmsis<Data>& pool) : ActorCmsis<Data>(name, queueSize, threadAttributes, pool), logic(pool) {}

bool MyActorCmsis::processMsg(Message<Data>* msg) {
    return logic.processMessage(this, msg);
};

Message<Data>* build(ActorData data, MessagePool<Data>& pool) {
    Data dataVariant{data};
    Message<Data>* msg = pool.acquire();
    msg->kind(dataVariant.data.index());
    msg->data(dataVariant);
    return msg;
}

/**
 * Actor One writes message to output and send a message to actor two.
 * Actor Two writes message to output and send a message to actor one.
 * Actor Three reads from input and send a message to Actor Two to change the message content.
 */
int main(int argc, char** argv) {
    osKernelInitialize();
    osKernelStart();

    MyActorCpp actor1("actor1", msgPoolCpp);
    MyActorCpp actor2("actor2", msgPoolCpp);
    MyActorCpp actor3("actor3", msgPoolCpp);
    char payload[64] = "this is a main payload";

    cout << "Message Pool C++   " << msgPoolCpp.nrOfFreeMsgs() << endl;
    cout << "Message Pool cmsis " << msgPoolCmsis.nrOfFreeMsgs() << endl;

    MyActorCpp::send(actor2, build({ActorData::ACTOR_COMMUNICATION, 0, payload, &actor1, &actor2}, msgPoolCpp));
    strcpy(payload, "this is a new payload");
    MyActorCpp::send(actor3, build({ActorData::ACTOR_CONFIGURATION, 0, payload, &actor3, &actor2}, msgPoolCpp));
    actor1.join();
    actor2.join();
    actor3.join();

    MyActorCmsis actorCmsis1("actorCmsis1", 10, msgPoolCmsis);
    MyActorCmsis actorCmsis2("actorCmsis2", 10, msgPoolCmsis);
    MyActorCmsis actorCmsis3("actor3", 10, msgPoolCmsis);
    strcpy(payload, "this is a main payload");
    MyActorCmsis::send(actorCmsis2, build({ActorData::ACTOR_COMMUNICATION, 0, payload, &actorCmsis1, &actorCmsis2}, msgPoolCmsis));
    strcpy(payload, "this is a new payload");
    MyActorCmsis::send(actorCmsis3, build({ActorData::ACTOR_CONFIGURATION, 0, payload, &actorCmsis3, &actorCmsis2}, msgPoolCmsis));
    actorCmsis1.join();
    actorCmsis2.join();
    actorCmsis3.join();

    cout << "Message Pool C++   " << msgPoolCpp.nrOfFreeMsgs() << endl;
    cout << "Message Pool cmsis " << msgPoolCmsis.nrOfFreeMsgs() << endl;

    return 0;
}
