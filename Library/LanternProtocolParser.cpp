#include "LanternProtocolParser.h"
#include "LanternCommands.h"

LanternProtocolParser::LanternProtocolParser() {
}

void LanternProtocolParser::parse(const QByteArray &chunk) {
    if (chunk.isEmpty())
        return;
    for (const char &byte : chunk) {
        _processByte(byte);
    }
}

std::shared_ptr<LanternCommand> LanternProtocolParser::fetch() {
    if (_commandsParsed.empty())
        return nullptr;
    auto result = _commandsParsed.front();
    _commandsParsed.pop();
    return result;
}

void LanternProtocolParser::_processByte(char byte) {
    switch (_state) {
    case State::WaitForType:
        assert(!_command);
        assert(_dataBuffer.isEmpty());
        _command = LanternCommand::createCommand(byte);
        _state = State::WaitForLength;
        _dataLeftSize = numBytesForLengthField;
        break;
    case State::WaitForLength:
        assert(_dataBuffer.size() < numBytesForLengthField);
        _dataBuffer.append(byte);
        _dataLeftSize -= 1;
        if (_dataBuffer.size() == numBytesForLengthField) {
            _dataLeftSize = _readBigEndian(_dataBuffer);
            //            _dataLeftSize = *reinterpret_cast<uint16_t *>(_dataBuffer.data());
            _dataBuffer.clear();
            if (0 == _dataLeftSize) {
                _finalizeCommandParsing();
            } else {
                _state = State::WaitForData;
            }
        }
        break;
    case State::WaitForData:
        assert(_dataLeftSize > 0);
        _dataBuffer.append(byte);
        _dataLeftSize -= 1;
        if (0 == _dataLeftSize) {
            _command->setData(_dataBuffer);
            _finalizeCommandParsing();
        }
        break;
    }
}

void LanternProtocolParser::_finalizeCommandParsing() {
    _dataBuffer.clear();
    _dataLeftSize = -1;
    _commandsParsed.push(_command);
    _command.reset();
    _state = State::WaitForType;
}

uint16_t LanternProtocolParser::_readBigEndian(const QByteArray &data) {
    if (data.size() != 2) {
        assert(false);
        // TODO: Error
        return 0;
    }
    uint16_t result;
    auto resultPtr = (char *)&result;
    resultPtr[0] = data[1];
    resultPtr[1] = data[0];
    return result;
}
