// This is a public header file, it must only include public header files.

#ifndef _XNET_COMMON_FILEUTIL_H_
#define _XNET_COMMON_FILEUTIL_H_

#include "StringPiece.h"
#include <boost/noncopyable.hpp>

namespace xnet
{

	namespace FileUtil
	{

		class SmallFile: boost::noncopyable
		{
		public:
			SmallFile(StringPiece filename);
			~SmallFile();

			// return errno
			template<typename String>
			int readToString(int maxSize, String* content, int64_t* fileSize, int64_t* modifyTime, int64_t* createTime);

			// return errno
			int readToBuffer(int* size);

			const char* buffer() const
			{
				return buf_;
			}

			static const int kBufferSize = 65536;

		private:
			int fd_;
			int err_;
			char buf_[kBufferSize];
		};

		// read the file content, returns errno if error happens.
		template<typename String>
		int readFile(StringPiece filename, int maxSize, String* content, int64_t* fileSize = NULL, int64_t* modifyTime = NULL, int64_t* createTime = NULL)
		{
			SmallFile file(filename);
			return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
		}

	}

}

#endif  // _XNET_COMMON_FILEUTIL_H_

