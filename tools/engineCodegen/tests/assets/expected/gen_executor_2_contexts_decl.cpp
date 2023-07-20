class Test
{
public:

private:

public:
    Context::LContext1::Data local1Context;
    Context::LContext2::Data local2Context;
    Context::LContext2::Data local3Context;
    Context::LContext1::Data target1Context;
    Context::LContext3::Data target2Context;
    Context::LContext3::Data target3Context;
    const Dod::SharedContext::Controller<Context::SContext1::Data>* shared1Context{ nullptr };
    const Dod::SharedContext::Controller<Context::SContext1::Data>* shared2Context{ nullptr };
    const Dod::SharedContext::Controller<Context::SContext2::Data>* shared3Context{ nullptr };
    const Dod::SharedContext::Controller<Context::SContext2::Data>* shared4Context{ nullptr };
};
