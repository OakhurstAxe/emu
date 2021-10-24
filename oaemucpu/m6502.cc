#include "headers/m6502.h"

#include <functional>
#include <QString>
#include <QDebug>

namespace oa
{
    namespace emu
    {
        
        M6502::M6502(IMemoryMapper *memory)
        {
            memory_ = memory;
            SetOpCodes();
            stackPointerMax_ = 0x1FF;
            stackPointerMin_ = 0xFF;
        }
        
        M6502::~M6502()
        {
        }
        
        void M6502::SetOpCodes()
        {
            opCodeLookup_[0x00] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x01] = {&M6502::OpORA, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0x02] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x03] = {&M6502::OpSLO, &M6502::IndirectXAddress, 8};
            opCodeLookup_[0x04] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x05] = {&M6502::OpORA, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x06] = {&M6502::OpASL, &M6502::ZeroAddress, 5};
            opCodeLookup_[0x07] = {&M6502::OpSLO, &M6502::ZeroAddress, 5};
            opCodeLookup_[0x08] = {&M6502::OpPHP, &M6502::NullAddress, 3};
            opCodeLookup_[0x09] = {&M6502::OpORA, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x0a] = {&M6502::OpASL, &M6502::Accumlator, 2};
            opCodeLookup_[0x0b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x0c] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x0d] = {&M6502::OpORA, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x0e] = {&M6502::OpASL, &M6502::AbsoluteAddress, 6};
            opCodeLookup_[0x0f] = {&M6502::OpSLO, &M6502::AbsoluteAddress, 6};

            opCodeLookup_[0x10] = {&M6502::OpBPL, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x11] = {&M6502::OpORA, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0x12] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x13] = {&M6502::OpSLO, &M6502::IndirectYAddressNoOverflow, 8};
            opCodeLookup_[0x14] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x15] = {&M6502::OpORA, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0x16] = {&M6502::OpASL, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0x17] = {&M6502::OpSLO, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0x18] = {&M6502::OpCLC, &M6502::NullAddress, 2};
            opCodeLookup_[0x19] = {&M6502::OpORA, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0x1a] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x1b] = {&M6502::OpSLO, &M6502::AbsoluteYAddressNoOverflow, 7};
            opCodeLookup_[0x1c] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x1d] = {&M6502::OpORA, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0x1e] = {&M6502::OpASL, &M6502::AbsoluteXAddressNoOverflow, 7};
            opCodeLookup_[0x1f] = {&M6502::OpSLO, &M6502::AbsoluteXAddressNoOverflow, 7};

            opCodeLookup_[0x20] = {&M6502::OpJSR, &M6502::AbsoluteAddress, 6};
            opCodeLookup_[0x21] = {&M6502::OpAND, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0x22] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x23] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x24] = {&M6502::OpBIT, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x25] = {&M6502::OpAND, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x26] = {&M6502::OpROL, &M6502::ZeroAddress, 5};
            opCodeLookup_[0x27] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x28] = {&M6502::OpPLP, &M6502::NullAddress, 4};
            opCodeLookup_[0x29] = {&M6502::OpAND, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x2a] = {&M6502::OpROL, &M6502::Accumlator, 2};
            opCodeLookup_[0x2b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x2c] = {&M6502::OpBIT, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x2d] = {&M6502::OpAND, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x2e] = {&M6502::OpROL, &M6502::AbsoluteAddress, 6};
            opCodeLookup_[0x2f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0x30] = {&M6502::OpBMI, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x31] = {&M6502::OpAND, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0x32] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x33] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x34] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x35] = {&M6502::OpAND, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0x36] = {&M6502::OpROL, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0x37] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x38] = {&M6502::OpSEC, &M6502::NullAddress, 2};
            opCodeLookup_[0x39] = {&M6502::OpAND, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0x3a] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x3b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x3c] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x3d] = {&M6502::OpAND, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0x3e] = {&M6502::OpROL, &M6502::AbsoluteXAddressNoOverflow, 7};
            opCodeLookup_[0x3f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0x40] = {&M6502::OpRTI, &M6502::NullAddress, 6};
            opCodeLookup_[0x41] = {&M6502::OpEOR, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0x42] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x43] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x44] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x45] = {&M6502::OpEOR, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x46] = {&M6502::OpLSR, &M6502::ZeroAddress, 5};
            opCodeLookup_[0x47] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x48] = {&M6502::OpPHA, &M6502::NullAddress, 3};
            opCodeLookup_[0x49] = {&M6502::OpEOR, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x4a] = {&M6502::OpLSR, &M6502::Accumlator, 2};
            opCodeLookup_[0x4b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x4c] = {&M6502::OpJMP, &M6502::AbsoluteAddress, 3};
            opCodeLookup_[0x4d] = {&M6502::OpEOR, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x4e] = {&M6502::OpLSR, &M6502::AbsoluteAddress, 6};
            opCodeLookup_[0x4f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0x50] = {&M6502::OpBVC, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x51] = {&M6502::OpEOR, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0x52] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x53] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x54] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x55] = {&M6502::OpEOR, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0x56] = {&M6502::OpLSR, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0x57] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x58] = {&M6502::OpCLI, &M6502::NullAddress, 2};
            opCodeLookup_[0x59] = {&M6502::OpEOR, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0x5a] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x5b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x5c] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x5d] = {&M6502::OpEOR, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0x5e] = {&M6502::OpLSR, &M6502::AbsoluteXAddressNoOverflow, 7};
            opCodeLookup_[0x5f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0x60] = {&M6502::OpRTS, &M6502::NullAddress, 6};
            opCodeLookup_[0x61] = {&M6502::OpADC, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0x62] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x63] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x64] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x65] = {&M6502::OpADC, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x66] = {&M6502::OpROR, &M6502::ZeroAddress, 5};
            opCodeLookup_[0x67] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x68] = {&M6502::OpPLA, &M6502::NullAddress, 4};
            opCodeLookup_[0x69] = {&M6502::OpADC, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x6a] = {&M6502::OpROR, &M6502::Accumlator, 2};
            opCodeLookup_[0x6b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x6c] = {&M6502::OpJMP, &M6502::IndirectAddress, 5};
            opCodeLookup_[0x6d] = {&M6502::OpADC, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x6e] = {&M6502::OpROR, &M6502::AbsoluteAddress, 6};
            opCodeLookup_[0x6f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0x70] = {&M6502::OpBVS, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x71] = {&M6502::OpADC, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0x72] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x73] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x74] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x75] = {&M6502::OpADC, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0x76] = {&M6502::OpROR, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0x77] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x78] = {&M6502::OpSEI, &M6502::NullAddress, 2};
            opCodeLookup_[0x79] = {&M6502::OpADC, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0x7a] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x7b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x7c] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x7d] = {&M6502::OpADC, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0x7e] = {&M6502::OpROR, &M6502::AbsoluteXAddressNoOverflow, 7};
            opCodeLookup_[0x7f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0x80] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x81] = {&M6502::OpSTA, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0x82] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x83] = {&M6502::OpSAX, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0x84] = {&M6502::OpSTY, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x85] = {&M6502::OpSTA, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x86] = {&M6502::OpSTX, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x87] = {&M6502::OpSAX, &M6502::ZeroAddress, 3};
            opCodeLookup_[0x88] = {&M6502::OpDEY, &M6502::NullAddress, 2};
            opCodeLookup_[0x89] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x8a] = {&M6502::OpTXA, &M6502::NullAddress, 2};
            opCodeLookup_[0x8b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x8c] = {&M6502::OpSTY, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x8d] = {&M6502::OpSTA, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x8e] = {&M6502::OpSTX, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0x8f] = {&M6502::OpSAX, &M6502::AbsoluteAddress, 4};

            opCodeLookup_[0x90] = {&M6502::OpBCC, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0x91] = {&M6502::OpSTA, &M6502::IndirectYAddressNoOverflow, 6};
            opCodeLookup_[0x92] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x93] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x94] = {&M6502::OpSTY, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0x95] = {&M6502::OpSTA, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0x96] = {&M6502::OpSTX, &M6502::ZeroYAddress, 4};
            opCodeLookup_[0x97] = {&M6502::OpSAX, &M6502::ZeroYAddress, 4};
            opCodeLookup_[0x98] = {&M6502::OpTYA, &M6502::NullAddress, 2};
            opCodeLookup_[0x99] = {&M6502::OpSTA, &M6502::AbsoluteYAddressNoOverflow, 5};
            opCodeLookup_[0x9a] = {&M6502::OpTXS, &M6502::NullAddress, 2};
            opCodeLookup_[0x9b] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x9c] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x9d] = {&M6502::OpSTA, &M6502::AbsoluteXAddressNoOverflow, 5};
            opCodeLookup_[0x9e] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0x9f] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0xa0] = {&M6502::OpLDY, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xa1] = {&M6502::OpLDA, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0xa2] = {&M6502::OpLDX, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xa3] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xa4] = {&M6502::OpLDY, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xa5] = {&M6502::OpLDA, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xa6] = {&M6502::OpLDX, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xa7] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xa8] = {&M6502::OpTAY, &M6502::NullAddress, 2};
            opCodeLookup_[0xa9] = {&M6502::OpLDA, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xaa] = {&M6502::OpTAX, &M6502::NullAddress, 2};
            opCodeLookup_[0xab] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xac] = {&M6502::OpLDY, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xad] = {&M6502::OpLDA, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xae] = {&M6502::OpLDX, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xaf] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0xb0] = {&M6502::OpBCS, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xb1] = {&M6502::OpLDA, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0xb2] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xb3] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xb4] = {&M6502::OpLDY, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0xb5] = {&M6502::OpLDA, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0xb6] = {&M6502::OpLDX, &M6502::ZeroYAddress, 4};
            opCodeLookup_[0xb7] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xb8] = {&M6502::OpCLV, &M6502::NullAddress, 2};
            opCodeLookup_[0xb9] = {&M6502::OpLDA, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0xba] = {&M6502::OpTSX, &M6502::NullAddress, 2};
            opCodeLookup_[0xbb] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xbc] = {&M6502::OpLDY, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0xbd] = {&M6502::OpLDA, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0xbe] = {&M6502::OpLDX, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0xbf] = {&M6502::OpBRK, &M6502::NullAddress, 7};

            opCodeLookup_[0xc0] = {&M6502::OpCPY, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xc1] = {&M6502::OpCMP, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0xc2] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xc3] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xc4] = {&M6502::OpCPY, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xc5] = {&M6502::OpCMP, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xc6] = {&M6502::OpDEC, &M6502::ZeroAddress, 5};
            opCodeLookup_[0xc7] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xc8] = {&M6502::OpINY, &M6502::NullAddress, 2};
            opCodeLookup_[0xc9] = {&M6502::OpCMP, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xca] = {&M6502::OpDEX, &M6502::NullAddress, 2};
            opCodeLookup_[0xcb] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xcc] = {&M6502::OpCPY, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xcd] = {&M6502::OpCMP, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xce] = {&M6502::OpDEC, &M6502::AbsoluteAddress, 3};
            opCodeLookup_[0xcf] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            
            opCodeLookup_[0xd0] = {&M6502::OpBNE, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xd1] = {&M6502::OpCMP, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0xd2] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xd3] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xd4] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xd5] = {&M6502::OpCMP, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0xd6] = {&M6502::OpDEC, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0xd7] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xd8] = {&M6502::OpCLD, &M6502::NullAddress, 2};
            opCodeLookup_[0xd9] = {&M6502::OpCMP, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0xda] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xdb] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xdc] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xdd] = {&M6502::OpCMP, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0xde] = {&M6502::OpDEC, &M6502::AbsoluteXAddressNoOverflow, 7};
            opCodeLookup_[0xdf] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            
            opCodeLookup_[0xe0] = {&M6502::OpCPX, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xe1] = {&M6502::OpSBC, &M6502::IndirectXAddress, 6};
            opCodeLookup_[0xe2] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xe3] = {&M6502::OpISC, &M6502::IndirectXAddress, 8};
            opCodeLookup_[0xe4] = {&M6502::OpCPX, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xe5] = {&M6502::OpSBC, &M6502::ZeroAddress, 3};
            opCodeLookup_[0xe6] = {&M6502::OpINC, &M6502::ZeroAddress, 5};
            opCodeLookup_[0xe7] = {&M6502::OpISC, &M6502::ZeroAddress, 5};
            opCodeLookup_[0xe8] = {&M6502::OpINX, &M6502::NullAddress, 2};
            opCodeLookup_[0xe9] = {&M6502::OpSBC, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xea] = {&M6502::OpNOP, &M6502::NullAddress, 2};
            opCodeLookup_[0xeb] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xec] = {&M6502::OpCPX, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xed] = {&M6502::OpSBC, &M6502::AbsoluteAddress, 4};
            opCodeLookup_[0xee] = {&M6502::OpINC, &M6502::AbsoluteAddress, 6};
            opCodeLookup_[0xef] = {&M6502::OpISC, &M6502::AbsoluteAddress, 6};

            opCodeLookup_[0xf0] = {&M6502::OpBEQ, &M6502::ImmediateAddress, 2};
            opCodeLookup_[0xf1] = {&M6502::OpSBC, &M6502::IndirectYAddress, 5};
            opCodeLookup_[0xf2] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xf3] = {&M6502::OpISC, &M6502::IndirectYAddressNoOverflow, 4};
            opCodeLookup_[0xf4] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xf5] = {&M6502::OpSBC, &M6502::ZeroXAddress, 4};
            opCodeLookup_[0xf6] = {&M6502::OpINC, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0xf7] = {&M6502::OpISC, &M6502::ZeroXAddress, 6};
            opCodeLookup_[0xf8] = {&M6502::OpSED, &M6502::NullAddress, 2};
            opCodeLookup_[0xf9] = {&M6502::OpSBC, &M6502::AbsoluteYAddress, 4};
            opCodeLookup_[0xfa] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xfb] = {&M6502::OpISC, &M6502::AbsoluteYAddressNoOverflow, 7};
            opCodeLookup_[0xfc] = {&M6502::OpBRK, &M6502::NullAddress, 7};
            opCodeLookup_[0xfd] = {&M6502::OpSBC, &M6502::AbsoluteXAddress, 4};
            opCodeLookup_[0xfe] = {&M6502::OpINC, &M6502::AbsoluteXAddressNoOverflow, 7};
            opCodeLookup_[0xff] = {&M6502::OpISC, &M6502::AbsoluteXAddressNoOverflow, 7};
        }
        
        void M6502::ExecuteTick()
        {
            if (overflowTicks_ > 1)
            {
                overflowTicks_--;
                return;
            }
            overflowTicks_--;

            CallOpMethod(operation_.operation_, operation_.addressMethod_);
            
            uint8_t instruction = memory_->CpuRead(programCounter_);
            programCounter_++;
            operation_ = opCodeLookup_[instruction];
            prevInst5 = prevInst4;
            prevInst4 = prevInst3;
            prevInst3 = prevInst2;
            prevInst2 = prevInst1;
            prevInst1 = instruction;
            
            // Add ticks for next instruction
            overflowTicks_ += operation_.ticks;
            SetOverflowForOperation();
            SetOverflowForAddressAccess();
        }

        void M6502::SetOverflowForOperation()
        {
            // If branch operaion takes a branch it causes extra tick
            if ((operation_.operation_ == &M6502::OpBCC && statusRegister_.carryFlag == false)
                || (operation_.operation_ == &M6502::OpBCS && statusRegister_.carryFlag == true)
                || (operation_.operation_ == &M6502::OpBEQ && statusRegister_.zeroFlag == true)
                || (operation_.operation_ == &M6502::OpBMI && statusRegister_.negativeFlag == true)
                || (operation_.operation_ == &M6502::OpBNE && statusRegister_.zeroFlag == false)
                || (operation_.operation_ == &M6502::OpBPL && statusRegister_.negativeFlag == false)
                || (operation_.operation_ == &M6502::OpBVC && statusRegister_.overflowFlag == false)
                || (operation_.operation_ == &M6502::OpBVS && statusRegister_.overflowFlag == true))
            {
                overflowTicks_++;
                int8_t relativeAddress = CallAddressMethod(operation_.addressMethod_);
                if (((programCounter_ & 0x00FF) + relativeAddress) > 0x00FF)
                {
                    overflowTicks_++;
                }
                programCounter_--;
            }
        }
        
        void M6502::SetOverflowForAddressAccess()
        {
            uint16_t carry = 0;
            
            // overflow on address lookup ONLY if low byte carrys to 
            // high byte by adding X or Y register
            if (operation_.addressMethod_ == &M6502::AbsoluteXAddress)
            {
                uint8_t loadl = memory_->CpuRead(programCounter_);
                carry = loadl + registerX_;
            }
            else if (operation_.addressMethod_ == &M6502::AbsoluteYAddress)
            {
                uint8_t loadl = memory_->CpuRead(programCounter_);
                carry = loadl + registerY_;
            }
            else if (operation_.addressMethod_ == &M6502::IndirectYAddress)
            {
                uint16_t indirect = memory_->CpuRead(programCounter_);
                uint8_t loadl = memory_->CpuRead(indirect & 0xff);
                carry = loadl + registerY_;
            }
            
            // Carry goes into high byte, so requires extra clock cycle
            if (carry > 0x00FF)
            {
                overflowTicks_ += 1;
            }
        }

        void M6502::PushStack(uint8_t byte)
        {
            if (stackPointer_ < stackPointerMin_)
            {
                // Atari VCS games use stack in strange ways, this check cannot be used there
                throw std::out_of_range(QString("Stack overflow").toLocal8Bit().data());
            }
            memory_->CpuWrite(stackPointer_, byte);
            stackPointer_--;
        }

        uint8_t M6502::PopStack(void)
        {
            if (stackPointer_ >= stackPointerMax_)
            {
                // Atari VCS games use stack in strange ways, this check cannot be used there
                throw std::out_of_range(QString("Stack underflow").toLocal8Bit().data());
            }
            stackPointer_++;
            return memory_->CpuRead(stackPointer_);
        }

        void M6502::Reset()
        {
            operation_ = opCodeLookup_[0xea]; //OpNOP
            
            stackPointer_ = stackPointerMax_;
            uint8_t pcl = memory_->CpuRead(0xfffc);
            uint8_t pch = memory_->CpuRead(0xfffd);
            programCounter_ = (pch << 8) + pcl;
            accumulator_ = 0;
            registerX_ = 0;
            registerY_ = 0;
            statusRegister_.interruptDisable = 1;
        }

        uint16_t M6502::NullAddress()
        {
            return 0;
        }
        uint16_t M6502::Accumlator()
        {
            return accumulator_;
        }
        uint16_t M6502::ImmediateAddress()
        {
            uint16_t address = programCounter_;
            programCounter_++;
            return address;
        }
        uint16_t M6502::ZeroAddress()
        {
            uint16_t address = memory_->CpuRead(programCounter_) & 0xFF;
            programCounter_++;
            return address;
        }

        uint16_t M6502::ZeroXAddress()
        {
            uint16_t address = (memory_->CpuRead(programCounter_) + registerX_) & 0xFF;
            programCounter_++;
            return address;
        }
        uint16_t M6502::ZeroYAddress()
        {
            uint16_t address = (memory_->CpuRead(programCounter_) + registerY_) & 0xFF;
            programCounter_++;
            return address;
        }
        uint16_t M6502::AbsoluteAddress()
        {
            uint8_t loadl = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint8_t loadh = memory_->CpuRead(programCounter_);
            programCounter_++;
            return (loadh << 8) + loadl;
        }
        uint16_t M6502::AbsoluteXAddress()
        {
            uint8_t loadl = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint8_t loadh = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint16_t address = (loadh << 8) + loadl + registerX_;
            return address;
        }
        uint16_t M6502::AbsoluteXAddressNoOverflow()
        {
            return AbsoluteXAddress();
        }
        uint16_t M6502::AbsoluteYAddress()
        {
            uint8_t loadl = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint8_t loadh = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint16_t address = (loadh << 8) + loadl + registerY_;
            return address;
        }
        uint16_t M6502::AbsoluteYAddressNoOverflow()
        {
            return AbsoluteYAddress();
        }
        uint16_t M6502::IndirectAddress()
        {
            uint8_t loadl = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint8_t loadh = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint16_t load = (loadh << 8) + loadl;
            // Bug in M6502 if low nibble is end of page
            if (loadl == 0x00ff)
            {
                loadl = memory_->CpuRead(load);
                loadh = memory_->CpuRead(load & 0xff00);
            }
            else
            {
                loadl = memory_->CpuRead(load);
                load++;
                loadh = memory_->CpuRead(load);
            }
            return (loadh << 8) + loadl;
        }
        uint16_t M6502::IndirectXAddress()
        {
            uint16_t indirect = (memory_->CpuRead(programCounter_) + registerX_);
            programCounter_++;
            uint8_t loadl = memory_->CpuRead(indirect & 0xff);
            indirect++;
            uint8_t loadh = memory_->CpuRead(indirect & 0xff);
            return (loadh << 8) + loadl;
        }
        uint16_t M6502::IndirectYAddress()
        {
            uint16_t indirect = memory_->CpuRead(programCounter_);
            programCounter_++;
            uint8_t loadl = memory_->CpuRead(indirect & 0xff);
            indirect++;
            uint8_t loadh = memory_->CpuRead(indirect & 0xff);
            uint16_t address = (loadh << 8) + loadl;
            address += registerY_;
            return address;            
        }
        uint16_t M6502::IndirectYAddressNoOverflow()
        {
            return IndirectYAddress();
        }

        // Load Store operations
        void M6502::OpLDA(AddressMethod addressMethod) 
        {
            uint16_t address = CallAddressMethod(addressMethod);
            accumulator_ = memory_->CpuRead(address);
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        void M6502::OpLDX(AddressMethod addressMethod) 
        {
            registerX_ = memory_->CpuRead(CallAddressMethod(addressMethod));
            statusRegister_.negativeFlag = (registerX_ & 0x80) != 0;
            statusRegister_.zeroFlag = (registerX_ == 0);
        }
        void M6502::OpLDY(AddressMethod addressMethod) 
        {
            registerY_ = memory_->CpuRead(CallAddressMethod(addressMethod));
            statusRegister_.negativeFlag = (registerY_ & 0x80) != 0;
            statusRegister_.zeroFlag = (registerY_ == 0);
        }
        void M6502::OpSTA(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            memory_->CpuWrite(address, accumulator_);
        }
        void M6502::OpSTX(AddressMethod addressMethod)
        {
            memory_->CpuWrite(CallAddressMethod(addressMethod), registerX_);
        }
        void M6502::OpSTY(AddressMethod addressMethod)
        {
            memory_->CpuWrite(CallAddressMethod(addressMethod), registerY_);
        }

        // Register transfers
        void M6502::OpTAX(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            registerX_ = accumulator_;
            statusRegister_.negativeFlag = (registerX_ & 0x80) != 0;
            statusRegister_.zeroFlag = (registerX_ == 0);
        }
        void M6502::OpTAY(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            registerY_ = accumulator_;
            statusRegister_.negativeFlag = (registerY_ & 0x80) != 0;
            statusRegister_.zeroFlag = (registerY_ == 0);
        }
        void M6502::OpTXA(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            accumulator_ = registerX_;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        void M6502::OpTYA(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            accumulator_ = registerY_;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }

        // Stack operaions
        void M6502::OpTSX(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            registerX_ = stackPointer_ & 0xFF;
            statusRegister_.negativeFlag = (registerX_ & 0x80) != 0;
            statusRegister_.zeroFlag = (registerX_ == 0);
        }
        void M6502::OpTXS(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            stackPointer_ = registerX_ | 0x100;
        }
        void M6502::OpPHA(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            PushStack(accumulator_);
        }
        void M6502::OpPHP(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            statusRegister_.breakCommand = 1;
            statusRegister_.ignored = 1;
            PushStack(statusRegister_.register_);
            statusRegister_.breakCommand = 0;
            statusRegister_.ignored = 0;
        }
        void M6502::OpPLA(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            accumulator_ = PopStack();
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        void M6502::OpPLP(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            statusRegister_.register_ = PopStack();
        }    

        // Logical operations
        void M6502::OpAND(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            accumulator_ &= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        void M6502::OpEOR(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            accumulator_ ^= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        void M6502::OpORA(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            accumulator_ |= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) != 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        void M6502::OpBIT(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            statusRegister_.zeroFlag = (((byte & accumulator_) & 0xff) == 0);
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
            statusRegister_.overflowFlag = (byte & 0x40) != 0;
        }
            
        // Arithmetic operations
        void M6502::OpADC(AddressMethod addressMethod)
        {
            uint16_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            uint8_t tmpAccumulator = accumulator_;
            uint16_t value;
            if (statusRegister_.decimalMode)
            {
                byte = (((byte & 0xF0) >> 4) * 10) + (byte & 0x0F) + statusRegister_.carryFlag;
                tmpAccumulator = (((tmpAccumulator & 0xF0) >> 4) * 10) + (tmpAccumulator & 0x0F);
                bool newCarry = (tmpAccumulator + byte) > 99;
                value = tmpAccumulator + byte;
                if (newCarry)
                {
                    value -= 100;
                }
                statusRegister_.carryFlag = newCarry;
                value = ((value / 10) << 4) + (value % 10);
            }
            else
            {
                value = tmpAccumulator + byte + statusRegister_.carryFlag;
                statusRegister_.carryFlag = (value > 255);
            }
            statusRegister_.zeroFlag = ((value & 0xff) == 0);
            statusRegister_.overflowFlag = (~(accumulator_ ^ byte) & (accumulator_ ^ value) & 0x80) != 0;
            statusRegister_.negativeFlag = (value & 0x80) != 0;
            accumulator_ = value & 0xff;
        }  
        void M6502::OpSBC(AddressMethod addressMethod)
        {
            uint16_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            uint8_t tmpAccumulator = accumulator_;
            uint16_t value;
            if (statusRegister_.decimalMode)
            {
                byte = (((byte & 0xF0) >> 4) * 10) + (byte & 0x0F);
                if (!statusRegister_.carryFlag)
                {
                    byte++;
                }
                tmpAccumulator = (((tmpAccumulator & 0xF0) >> 4) * 10) + (tmpAccumulator & 0x0F);
                bool newCarry = tmpAccumulator >= byte;
                if (tmpAccumulator < byte)
                {
                    tmpAccumulator += 100;
                }
                value = tmpAccumulator - byte;
                statusRegister_.carryFlag = newCarry;
                value = ((value / 10) << 4) + (value % 10);
            }
            else
            {
                byte ^= 0xff;
                value = tmpAccumulator + byte + statusRegister_.carryFlag;
                statusRegister_.carryFlag = (value & 0xff00) != 0;
            }
            statusRegister_.zeroFlag = (value & 0xff) == 0;
            statusRegister_.overflowFlag  = ((value ^ accumulator_) & (value ^ byte) & 0x80) != 0;
            statusRegister_.negativeFlag = (value & 0x80) != 0;
            accumulator_ = value & 0xff;
        }
        void M6502::OpCMP(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            statusRegister_.carryFlag = (accumulator_ >= byte);
            statusRegister_.zeroFlag = (accumulator_ == byte);
            statusRegister_.negativeFlag = ((accumulator_ - byte) & 0x80) != 0;
        }
        void M6502::OpCPX(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            statusRegister_.carryFlag = (registerX_ >= byte);
            statusRegister_.zeroFlag = (registerX_ == byte);
            statusRegister_.negativeFlag = ((registerX_ - byte) & 0x80) != 0;
        }
        void M6502::OpCPY(AddressMethod addressMethod)
        {
            uint8_t byte = memory_->CpuRead(CallAddressMethod(addressMethod));
            statusRegister_.carryFlag = (registerY_ >= byte);
            statusRegister_.zeroFlag = (registerY_ == byte);
            statusRegister_.negativeFlag = ((registerY_ - byte) & 0x80) != 0;
        }

        // Increment and decrement operations
        void M6502::OpINC(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            byte++;
            memory_->CpuWrite(address, byte);
            statusRegister_.zeroFlag = (byte == 0);
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
        }
        void M6502::OpINX(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            registerX_++;
            statusRegister_.zeroFlag = (registerX_ == 0);
            statusRegister_.negativeFlag = (registerX_ & 0x80) != 0;
        }
        void M6502::OpINY(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            registerY_++;
            statusRegister_.zeroFlag = (registerY_ == 0);
            statusRegister_.negativeFlag = (registerY_ & 0x80) != 0;
        }
        void M6502::OpDEC(AddressMethod addressMethod) 
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            byte--;
            memory_->CpuWrite(address, byte);
            statusRegister_.zeroFlag = (byte == 0);
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
        }
        void M6502::OpDEX(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            registerX_--;
            statusRegister_.zeroFlag = (registerX_ == 0);
            statusRegister_.negativeFlag = (registerX_ & 0x80) != 0;
        }
        void M6502::OpDEY(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            registerY_--;
            statusRegister_.zeroFlag = (registerY_ == 0);
            statusRegister_.negativeFlag = (registerY_ & 0x80) != 0;
        }

        // Shift operations
        void M6502::OpASL(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            if (addressMethod == &M6502::Accumlator)
            {
                byte = accumulator_;// CallAddressMethod(addressMethod);
            }
            else
            {
                address = CallAddressMethod(addressMethod);
                byte = memory_->CpuRead(address);
            }
            statusRegister_.carryFlag = (byte & 0x80) != 0;
            byte = byte << 1;
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
            statusRegister_.zeroFlag = (byte == 0);
            if (addressMethod == &M6502::Accumlator)
            {
                accumulator_ = byte;
            }
            else
            {
                memory_->CpuWrite(address, byte);
            }
        }    

        void M6502::OpLSR(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            if (addressMethod == &M6502::Accumlator)
            {
                byte = CallAddressMethod(addressMethod);
            }
            else
            {
                address = CallAddressMethod(addressMethod);
                byte = memory_->CpuRead(address);
            }
            statusRegister_.carryFlag = (byte & 0x01) != 0;
            byte = byte >> 1;
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
            statusRegister_.zeroFlag = (byte == 0);
            if (addressMethod == &M6502::Accumlator)
            {
                accumulator_  = byte;
            }
            else
            {
                memory_->CpuWrite(address, byte);
            }
        }
        void M6502::OpROL(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            if (addressMethod == &M6502::Accumlator)
            {
                byte = CallAddressMethod(addressMethod);
            }
            else
            {
                address = CallAddressMethod(addressMethod);
                byte = memory_->CpuRead(address);
            }
            uint8_t temp = (byte << 1) + statusRegister_.carryFlag;
            statusRegister_.carryFlag = (byte & 0x80) != 0;
            byte = temp;
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
            statusRegister_.zeroFlag = (byte == 0);
            if (addressMethod == &M6502::Accumlator)
            {
                accumulator_  = byte;
            }
            else
            {
                memory_->CpuWrite(address, byte);
            }
        }
            
        void M6502::OpROR(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            if (addressMethod == &M6502::Accumlator)
            {
                byte = CallAddressMethod(addressMethod);
            }
            else
            {
                address = CallAddressMethod(addressMethod);
                byte = memory_->CpuRead(address);
            }
            uint8_t temp = (byte >> 1) + (statusRegister_.carryFlag << 7);
            statusRegister_.carryFlag = (byte & 0x01) != 0;
            byte = temp;
            statusRegister_.negativeFlag = (byte & 0x80) != 0;
            statusRegister_.zeroFlag = (byte == 0);
            if (addressMethod == &M6502::Accumlator)
            {
                accumulator_  = byte;
            }
            else
            {
                memory_->CpuWrite(address, byte);
            }
        }

        // Jumps and Call operaions
        void M6502::OpJMP(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            programCounter_ = address;
        }
        void M6502::OpJSR(AddressMethod addressMethod) 
        {
            uint16_t jumpAddress = CallAddressMethod(addressMethod);
            programCounter_--;
            PushStack((programCounter_ & 0xff00) >> 8);
            PushStack(programCounter_ & 0xff);
            programCounter_ = jumpAddress;
        }
        void M6502::OpRTS(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            uint8_t loadl = PopStack();
            uint8_t loadh = PopStack();
            uint16_t load = (loadh << 8) + loadl;
            programCounter_ = load;
            programCounter_++;
        }

        // branch operations
        void M6502::OpBCC(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.carryFlag == false)
            {
                programCounter_ += relativeAddress;
            }
        }
        void M6502::OpBCS(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.carryFlag == true)
            {
                programCounter_ += relativeAddress;
            }
        }
        void M6502::OpBEQ(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.zeroFlag == true)
            {
                programCounter_ += relativeAddress;
            }
        }
        void M6502::OpBMI(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.negativeFlag == true)
            {
                programCounter_ += relativeAddress;
            }
        }    
        void M6502::OpBNE(AddressMethod addressMethod) 
        { 
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.zeroFlag == false)
            {
                programCounter_ += relativeAddress;
            }
        }
        void M6502::OpBPL(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.negativeFlag == false)
            {
                programCounter_ += relativeAddress;
            }
        }
        void M6502::OpBVC(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.overflowFlag == false)
            {
                programCounter_ += relativeAddress;
            }
        }
        void M6502::OpBVS(AddressMethod addressMethod) 
        {
            int8_t relativeAddress = memory_->CpuRead(CallAddressMethod(addressMethod));
            if (statusRegister_.overflowFlag == true)
            {
                programCounter_ += relativeAddress;
            }
        }

        // Status Flag operations
        void M6502::OpCLC(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.carryFlag = false;
        }
        void M6502::OpCLD(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.decimalMode = false;
        }
        void M6502::OpCLI(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.interruptDisable = false;
        }
        void M6502::OpCLV(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.overflowFlag = false;
        }
        void M6502::OpSEC(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.carryFlag = true;
        }
        void M6502::OpSED(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.decimalMode = true;
        }
        void M6502::OpSEI(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.interruptDisable = true;
        }

        // System operations
        void M6502::OpBRK(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            statusRegister_.interruptDisable = 1;
            PushStack((programCounter_ & 0xff00) >> 8);
            PushStack(programCounter_ & 0x00ff);
            statusRegister_.breakCommand = true;
            PushStack(statusRegister_.register_);
            statusRegister_.breakCommand = false;
            uint8_t loadl = memory_->CpuRead(0xfffe);
            uint8_t loadh = memory_->CpuRead(0xffff);
            uint16_t load = (loadh << 8) + loadl;
            programCounter_ = load;
        }
        void M6502::OpNOP(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
        }
        void M6502::OpRTI(AddressMethod addressMethod)
        {
            Q_UNUSED(addressMethod);
            statusRegister_.register_ = PopStack();
            statusRegister_.breakCommand = false;
            uint8_t loadl = PopStack();
            uint8_t loadh = PopStack();
            uint16_t load = (loadh << 8) + loadl;
            programCounter_ = load;
        }
        
        void M6502::OpISC(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            byte++;
            memory_->CpuWrite(address, byte);
            //statusRegister_.zeroFlag = (byte == 0);
            //statusRegister_.negativeFlag = (byte & 0x80) > 0;
            
            byte = byte ^ 0xff;            
            uint16_t value = accumulator_ + byte + statusRegister_.carryFlag;
            statusRegister_.carryFlag = (value & 0xff00) > 0;
            statusRegister_.zeroFlag = (value & 0xff) == 0;
            statusRegister_.overflowFlag  = ((value ^ accumulator_) & (value ^ byte) & 0x80) > 0;
            statusRegister_.negativeFlag = (value & 0x80) > 0;
            accumulator_ = value & 0xff;            
        }
        
        void M6502::OpSAX(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = accumulator_ & registerX_;
            memory_->CpuWrite(address, byte);
        }

        void M6502::OpSLO(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            statusRegister_.carryFlag = (byte & 0x80) > 0;
            byte = byte << 1;
            memory_->CpuWrite(address, byte);
            
            accumulator_ |= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) > 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);            
        }        
    }
}
