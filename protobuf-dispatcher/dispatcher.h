#include <google/protobuf/message.h>

#include <map>
#include <functional>
#include<type_traits>

using MessagePtr = std::shared_ptr<google::protobuf::Message>;

class Callback {
public:
	virtual ~Callback() = default;
	virtual void onMessage(const MessagePtr& message) const = 0;
};

template <typename T>
class CallbackT : public Callback{
	static_assert(std::is_base_of<google::protobuf::Message, T>::value,
		"T must be derived from gdb::Messsage.");

public:
	using ProtobufMessageTCallback = std::function<void(const std::shared_ptr<T>& message)>;
	CallbackT(const ProtobufMessageTCallback& callback)
		:callback_(callback)
	{}

	void onMessage(const MessagePtr& message) const override {
		std::shared_ptr<T> concrete = std::static_pointer_cast<T>(message);
		assert(concrete != nullptr);
		callback_(concrete);
	}

private:
	ProtobufMessageTCallback callback_;
};

class ProtobufDispatcher
{
public:
	using ProtobufMessageCallback = std::function<void(const MessagePtr& message)>;
	
	explicit ProtobufDispatcher(const ProtobufMessageCallback& defaultCb)
		:defaultCallback_(defaultCb)
	{}

	void onProtobufMessage(const MessagePtr& message) const {
		const auto iter = callbacks_.find(message->GetDescriptor());
		if (iter != callbacks_.cend()) {
			iter->second->onMessage(message);
		}
		else {
			defaultCallback_(message);
		}
	}

	template<typename T>
	void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageTCallback& callback) {
		auto pd = std::make_shared<CallbackT<T>>(callback);
		callbacks_[T::descriptor()] = pd;
	}



private:
	using CallbackMap = std::map<const google::protobuf::Descriptor*, std::shared_ptr<Callback>>;

	CallbackMap callbacks_;
	const ProtobufMessageCallback defaultCallback_;
};

