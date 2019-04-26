#include "dispatcher.h"

#include "message.pb.h"

#include <iostream>

using std::cout;
using std::endl;

typedef std::shared_ptr<muduo::Query> QueryPtr;
typedef std::shared_ptr<muduo::Answer> AnswerPtr;

void test_down_pointer_cast()
{
	::std::shared_ptr<google::protobuf::Message> msg(new muduo::Query);
	::std::shared_ptr<muduo::Query> query(std::static_pointer_cast<muduo::Query>(msg));
	assert(msg && query);
	if (!query)
	{
		abort();
	}
}

void onQuery(const QueryPtr& message)
{
	cout << "onQuery: " << message->GetTypeName() << endl;
}

void onAnswer(const AnswerPtr& message)
{
	cout << "onAnswer: " << message->GetTypeName() << endl;
}

void onUnknownMessageType(const MessagePtr& message)
{
	cout << "onUnknownMessageType: " << message->GetTypeName() << endl;
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	test_down_pointer_cast();

	ProtobufDispatcher dispatcher(onUnknownMessageType);
	dispatcher.registerMessageCallback<muduo::Query>(onQuery);
	dispatcher.registerMessageCallback<muduo::Answer>(onAnswer);

	std::shared_ptr<muduo::Query> query(new muduo::Query);
	std::shared_ptr<muduo::Answer> answer(new muduo::Answer);
	std::shared_ptr<muduo::Empty> empty(new muduo::Empty);
	dispatcher.onProtobufMessage(query);
	dispatcher.onProtobufMessage(answer);
	dispatcher.onProtobufMessage(empty);

	google::protobuf::ShutdownProtobufLibrary();
}

