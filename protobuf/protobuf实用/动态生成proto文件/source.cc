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
	FileDescriptorProto file_proto;
	file_proto.set_name("my.proto");
	file_proto.set_syntax("proto3");

	DescriptorProto* message_proto = file_proto.add_message_type();
	message_proto->set_name("mymsg");

	FieldDescriptorProto* field_proto = NULL;

	field_proto = message_proto->add_field();
	field_proto->set_name("len");
	field_proto->set_type(FieldDescriptorProto::TYPE_UINT32);
	field_proto->set_number(1);
	field_proto->set_label(FieldDescriptorProto::LABEL_OPTIONAL);

	field_proto = message_proto->add_field();
	field_proto->set_name("type");
	field_proto->set_type(FieldDescriptorProto::TYPE_UINT32);
	field_proto->set_number(2);

	DescriptorPool pool;
	const FileDescriptor* file_descriptor = pool.BuildFile(file_proto);
	cout << file_descriptor->DebugString();
	return 0;
}