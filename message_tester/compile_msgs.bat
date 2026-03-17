REM protoc --python_out=. ../message_defs /message.proto

REM protoc --proto_path=. --python_out=. ../message_defs/message.proto

protoc  --proto_path=../message_defs   --python_out=. ../message_defs/message_defs.proto