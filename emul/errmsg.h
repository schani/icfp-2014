
const char const *fault_msg[FLT_LAST] = {
	[FLT_UNKNOWN] = "unknown fault",
	[FLT_TAG_MISMATCH_INT] = "not an INT type",
	[FLT_TAG_MISMATCH_CONS] = "not a CONS type",
	[FLT_TAG_MISMATCH_CLOSURE] = "not a CLOSURE type",
	[FLT_CONTROL_MISMATCH_JOIN] = "invalid join address",
	[FLT_CONTROL_MISMATCH_RET] = "invalid return address",
	[FLT_FRAME_MISMATCH_DUM] = "not an empty frame",
	[FLT_FRAME_MISMATCH_FRAME] = "not a DUM frame",
	[FLT_FRAME_MISMATCH_SIZE] = "invalid frame size",
	[FLT_FRAME_MISMATCH_SAME] = "environment not identical with frame pointer"
};

