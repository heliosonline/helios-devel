#pragma once

#include "Common/StdErrorCode.h"


namespace Net {

	// Error codes for LibNet
	enum class ErrorCode
	{
		Success = 0,

		MsgErrorHeaderCRC,
		MsgErrorBodyCRC,
		MsgErrorBodyEmpty,

		MaxErrorCodes
	};

} // namespace Net


// Implement std::error_code functionality
STDERRORCODE(Net, ErrorCode)
