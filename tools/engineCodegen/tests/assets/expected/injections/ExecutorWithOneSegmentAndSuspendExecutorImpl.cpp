#include "ExecutorWithOneSegmentAndSuspendExecutor.h"

using namespace Game::ExecutionBlock;

void ExecutorWithOneSegmentAndSuspend::initImpl() noexcept
{

}

void ExecutorWithOneSegmentAndSuspend::updateImpl([[maybe_unused]] float dt) noexcept
{

}

// BEGIN target1
void ExecutorWithOneSegmentAndSuspend::modifyTarget1([[maybe_unused]] Context::LContext1::Data& target1) noexcept
{

}
// END target1

/*// BEGIN target2
void ExecutorWithOneSegmentAndSuspend::modifyTarget2([[maybe_unused]] Context::LContext3::Data& target2) noexcept
{

}
*/// END target2
