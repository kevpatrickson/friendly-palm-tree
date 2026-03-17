import serial
from message_defs_pb2 import Item, Type, Msg

fields = Msg.DESCRIPTOR.fields
for f in fields:
    print(f.name, f.type)    

msg1=Item()
msg1.ID=10
msg1.enabled=False
msg1.parm1=20
msg1.parm2=40
msg1.parm3=50
msg1.parm4=60


bigMsg=Msg()

bigMsg.item.CopyFrom(msg1)
print(bigMsg.ListFields())



s=bigMsg.SerializeToString()
print(s.hex(" "))
print(type(s))


msg = Msg()
msg.ParseFromString(s)
print("Decoded: ", msg)



# a.item.ID=1

# a.item1.parm1=3

# print(a.WhichOneof("payload"))




# a=Item()
# a.ID=Type.TypeA
# a.enabled=True
# a.parm1=1.1
# a.parm2=1.2
# a.parm3=1.3
# a.parm4=1.4


# s=a.SerializeToString()
# print(s.hex(" "))
# print(type(s))

# print(" ".join(f", 0x{b:02x}" for b in s))



# with open("output.bin", "wb") as f:
#     f.write(s)



# ser = serial.Serial(
#     port="COM9",       # Change to your COM port
#     baudrate=115200,
#     timeout=1
# )

# data = b"\x01\x02\x03\xFF"   # Any bytes object
# ser.write(s)
