#include "Net/NetCommon.h"
#include "Net/NetErrors.h"


// Implement std::error_code functionality
STDERRORCODE_START(NetLib, Net, ErrorCode)
STDERRORCODE_ERROR(Net::ErrorCode::MsgErrorHeaderCRC,   "Checksum of header is not correct.")
STDERRORCODE_ERROR(Net::ErrorCode::MsgErrorBodyCRC,     "Checksum of body is not correct.")
STDERRORCODE_ERROR(Net::ErrorCode::MsgErrorBodyEmpty,   "Message body is empty.")
STDERRORCODE_END(NetLib, Net, ErrorCode)
