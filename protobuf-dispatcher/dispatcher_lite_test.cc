#include "dispatch_lite.h"
#include "message.pb.h"

#include<iostream>

using std::cout;
using std::endl;



void onUnknowMessageType(const MessagePtr& message) {
	cout << "onUnknowMessageType: " << message->GetTypeName() << endl;
}

void onQuery(const MessagePtr& message) {
	cout << "onQuery: " << message->GetTypeName() << endl;
	auto query = std::static_pointer_cast<muduo::Query>(message);
	assert(query != nullptr);
}

void onAnswer(const MessagePtr& message)
{
	cout << "onAnswer: " << message->GetTypeName() << endl;
	auto answer =std::static_pointer_cast<muduo::Answer>(message);
	assert(answer != NULL);
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	ProtobufDispatcherLite dispatcher(onUnknowMessageType);
	dispatcher.registerMessageCallback(muduo::Query::descriptor(), onQuery);
	dispatcher.registerMessageCallback(muduo::Answer::descriptor(), onAnswer);

	std::shared_ptr<muduo::Query> query(new muduo::Query);
	std::shared_ptr<muduo::Answer> answer(new muduo::Answer);
	std::shared_ptr<muduo::Empty> empty(new muduo::Empty);
	dispatcher.onProtobufMessage(query);
	dispatcher.onProtobufMessage(answer);
	dispatcher.onProtobufMessage(empty);

	google::protobuf::ShutdownProtobufLibrary();
}