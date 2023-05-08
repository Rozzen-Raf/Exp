#include "JsonParser.h"
#include "ApiCommand.h"

std::pair<ApiCommandBaseSharedPtr, JsonParser> ParseJsonApiCommand(buffer_view buffer);
