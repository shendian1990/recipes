#include"foo.pb.h"

#include <string>
#include <iostream>

using namespace std;
using namespace google::protobuf;

Message* createMessage(const string& name) {
	Message* message = nullptr;
	const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(name);
	if (descriptor != nullptr) {
		const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype != nullptr)
			message = prototype->New();
	}
	return message;
}

int main() {
	Message* message = createMessage("Foo");
	Foo* foo = dynamic_cast<Foo*>(message);
}