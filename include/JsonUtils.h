#include "JsonParser.h"
#include "ApiCommand.h"

std::pair<ApiCommandBaseSharedPtr, json> ParseJsonApiCommand(buffer_view buffer);
