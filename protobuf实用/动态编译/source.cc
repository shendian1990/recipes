//使用PB 提供的 google::protobuf::compiler 包在运行时动态编译指定的.proto 文件
//来使用其中的 Message。这样就可以通过修改.proto文件实现动态消息，
//有点类似配置文件的用法。完成这个工作主要的类叫做 importer，定义在 importer.h 中

#include <iostream>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::compiler;

int main(int argc, const char* argv[])
{
	DiskSourceTree sourceTree;
	//look up .proto file in current directory
	sourceTree.MapPath("", "./");
	Importer importer(&sourceTree, NULL);
	//runtime compile foo.proto
	importer.Import("pair.proto");

	const Descriptor* descriptor = importer.pool()->FindMessageTypeByName("Pair");
	cout << descriptor->DebugString();

	// build a dynamic message by "Pair" proto
	DynamicMessageFactory factory;
	const Message* message = factory.GetPrototype(descriptor);
	// create a real instance of "Pair"
	Message* pair = message->New();

	// write the "Pair" instance by reflection
	const Reflection* reflection = pair->GetReflection();

	const FieldDescriptor* field = NULL;
	field = descriptor->FindFieldByName("key");
	reflection->SetString(pair, field, "my key");
	field = descriptor->FindFieldByName("value");
	reflection->SetUInt32(pair, field, 1111);

	cout << pair->DebugString();

	delete pair;

	return 0;
}