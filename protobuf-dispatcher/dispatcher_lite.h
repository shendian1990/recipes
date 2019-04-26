#include <google/protobuf/message.h>

#include<functional>
#include <map>

using MessagePtr = std::shared_ptr<google::protobuf::Message>;

class ProtobufDispatcherLite {
public:
	using ProtobufMessageCallback = std::function<void(const MessagePtr&)>;
	explicit ProtobufDispatcherLite(const ProtobufMessageCallback& defaultCb)
		:defaultCallback_(defaultCb)
	{}

	void onProtobufMessage(const MessagePtr& message) const {
		CallbackMap::const_iterator iter = callbacks_.find(message->GetDescriptor());
		if (iter != callbacks_.cend()) {
			iter->second(message);
		}
		else
		{
			defaultCallback_(message);
		}
	}

	void registerMessageCallback(const google::protobuf::Descriptor* desc, const ProtobufMessageCallback& callback) {
		callbacks_[desc] = callback;
	}

private:
	using CallbackMap = std::map<const google::protobuf::Descriptor*, ProtobufMessageCallback>;
	CallbackMap callbacks_;
	const ProtobufMessageCallback defaultCallback_;
};