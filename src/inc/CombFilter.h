class FIR : public CCombFilterIf
{

}

class IIR : public CCombFilterIf
{
    public:
        static Error_t create (CCombFilterIf*& pCCombFilterIf) override;

}