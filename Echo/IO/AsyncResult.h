#pragma once

namespace Echo { namespace IO {

/**
 * Indicates the results of an asynchronous operation
 */
enum class AsyncResult
{
	Complete,
	Pending
};

}} // end of namespace