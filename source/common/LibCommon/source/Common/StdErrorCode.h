#pragma once
// ----------------------------------------------------------------------------
// Code is based on a blog post at...
// https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/
// ----------------------------------------------------------------------------

#include <system_error>
#include <vector>


// ----------------------------------------------------------------------------
// --[ Deklaration (header file) ]---------------------------------------------
// ----------------------------------------------------------------------------

#define STDERRORCODE(ns, ec_enum)                                                         \
	namespace std { template <> struct is_error_code_enum<ns::ec_enum> : true_type {}; }  \
	namespace ns {                                                                        \
		std::error_code make_error_code(ec_enum);                                         \
		std::error_code GetLastError();                                                   \
		void SetLastError(ec_enum ec);                                                    \
		void ClearLastError(bool resize = true);                                          \
	}


// ----------------------------------------------------------------------------
// --[ Definition (cpp file) ]-------------------------------------------------
// ----------------------------------------------------------------------------

// Implements std::error_code functionality
#define STDERRORCODE_START(catname, ns, ec_enum)                                          \
	namespace {                                                                           \
		struct ErrorCategory##catname : std::error_category {                             \
			const char* name() const noexcept override;                                   \
			std::string message(int ev) const override;                                   \
		};                                                                                \
		const char* ErrorCategory##catname::name() const noexcept { return #catname; }    \
		std::string ErrorCategory##catname::message(int ev) const {                       \
			switch (static_cast<ns::ec_enum>(ev)) {


// Implements a single error message/description
#define STDERRORCODE_ERROR(ns_enum_code, msg)                                             \
			case ns_enum_code: return msg;


// Implements:
//   std::error_code ns::GetLastError()
//   ns::SetLastError(ec_enum ec)
//   ns::ClearLastError()
#define STDERRORCODE_END(catname, ns, ec_enum)                                            \
			default:                                                                      \
				return "(unrecognized error)";                                            \
		} }                                                                               \
		const ErrorCategory##catname globalErrorCategory##catname{};                      \
	}                                                                                     \
	namespace ns {                                                                        \
		std::error_code ns::make_error_code(ec_enum ec) {                                 \
			return { static_cast<int>(ec), globalErrorCategory##catname };                \
		}                                                                                 \
		std::vector<std::error_code> globalLastError##catname;                            \
		std::error_code GetLastError() {                                                  \
			if (globalLastError##catname.empty())                                         \
				return std::error_code();                                                 \
			std::error_code ec_tmp = globalLastError##catname.back();                     \
			globalLastError##catname.pop_back();                                          \
			return ec_tmp;                                                                \
		}                                                                                 \
		void SetLastError(ec_enum ec) {                                                   \
			globalLastError##catname.push_back(make_error_code(ec));                      \
		}                                                                                 \
		void ClearLastError(bool resize) {                                                \
			globalLastError##catname.clear();                                             \
			if (resize)                                                                   \
				globalLastError##catname.shrink_to_fit();                                 \
		}                                                                                 \
	}
