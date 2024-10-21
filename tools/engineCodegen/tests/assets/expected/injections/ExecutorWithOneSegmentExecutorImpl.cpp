#include "ExecutorWithOneSegmentExecutor.h"

using namespace Game::ExecutionBlock;

void ExecutorWithOneSegment::initImpl([[maybe_unused]] ExternalContexts& externalContexts) noexcept
{

}

void ExecutorWithOneSegment::updateImpl([[maybe_unused]] ExternalContexts& externalContexts, [[maybe_unused]] float dt) noexcept
{

}

// BEGIN target1
void ExecutorWithOneSegment::modifyTarget1([[maybe_unused]] const CExternalContexts& externalContexts, [[maybe_unused]] Context::LContext1::Data& target1) noexcept
{

}
// END target1
