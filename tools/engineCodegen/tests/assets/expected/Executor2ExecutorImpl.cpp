#include "Executor2Executor.h"

using namespace Game::ExecutionBlock;

void Executor2::initImpl([[maybe_unused]] ExternalContexts& externalContexts) noexcept
{

}

void Executor2::updateImpl([[maybe_unused]] ExternalContexts& externalContexts, [[maybe_unused]] float dt) noexcept
{

}

// BEGIN target1
void Executor2::modifyTarget1([[maybe_unused]] const CExternalContexts& externalContexts, [[maybe_unused]] Context::LContext1::Data& target1) noexcept
{

}
// END target1

// BEGIN target2
void Executor2::modifyTarget2([[maybe_unused]] const CExternalContexts& externalContexts, [[maybe_unused]] Context::LContext3::Data& target2) noexcept
{

}
// END target2

// BEGIN target3
void Executor2::modifyTarget3([[maybe_unused]] const CExternalContexts& externalContexts, [[maybe_unused]] Context::LContext3::Data& target3) noexcept
{

}
// END target3
