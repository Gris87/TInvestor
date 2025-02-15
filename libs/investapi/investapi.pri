# find libs/investapi/ -name \*.cc | sed "s/libs\/investapi\///g" | sed "s/\.cc/\.cc \\\/g"
SOURCES += \
    messages/generated/common.grpc.pb.cc \
    messages/generated/common.pb.cc \
    messages/generated/google/api/field_behavior.grpc.pb.cc \
    messages/generated/google/api/field_behavior.pb.cc \
    messages/generated/instruments.grpc.pb.cc \
    messages/generated/instruments.pb.cc \
    messages/generated/marketdata.grpc.pb.cc \
    messages/generated/marketdata.pb.cc \
    messages/generated/operations.grpc.pb.cc \
    messages/generated/operations.pb.cc \
    messages/generated/orders.grpc.pb.cc \
    messages/generated/orders.pb.cc \
    messages/generated/sandbox.grpc.pb.cc \
    messages/generated/sandbox.pb.cc \
    messages/generated/signals.grpc.pb.cc \
    messages/generated/signals.pb.cc \
    messages/generated/stoporders.grpc.pb.cc \
    messages/generated/stoporders.pb.cc \
    messages/generated/users.grpc.pb.cc \
    messages/generated/users.pb.cc \

# find libs/investapi/ -name \*.h | sed "s/libs\/investapi\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    messages/generated/common.grpc.pb.h \
    messages/generated/common.pb.h \
    messages/generated/google/api/field_behavior.grpc.pb.h \
    messages/generated/google/api/field_behavior.pb.h \
    messages/generated/instruments.grpc.pb.h \
    messages/generated/instruments.pb.h \
    messages/generated/marketdata.grpc.pb.h \
    messages/generated/marketdata.pb.h \
    messages/generated/operations.grpc.pb.h \
    messages/generated/operations.pb.h \
    messages/generated/orders.grpc.pb.h \
    messages/generated/orders.pb.h \
    messages/generated/sandbox.grpc.pb.h \
    messages/generated/sandbox.pb.h \
    messages/generated/signals.grpc.pb.h \
    messages/generated/signals.pb.h \
    messages/generated/stoporders.grpc.pb.h \
    messages/generated/stoporders.pb.h \
    messages/generated/users.grpc.pb.h \
    messages/generated/users.pb.h \
