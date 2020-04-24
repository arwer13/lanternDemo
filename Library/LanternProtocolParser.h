#pragma once
#include <QByteArray>
#include <memory>
#include <queue>

class LanternCommand;

class LanternProtocolParser {
public:
    LanternProtocolParser();

    //! Feed parser with another portion of data
    void parse(const QByteArray &chunk);

    //! Fetch next command parsed or empty pointer if no command is ready
    std::shared_ptr<LanternCommand> fetch();

protected:
    enum class State {
        WaitForType,
        WaitForLength,
        WaitForData,
    } _state
        = State::WaitForType;

    typedef uint16_t LengthFieldType;
    static const constexpr int numBytesForLengthField = sizeof(LengthFieldType);

    void _processByte(char byte);

    //! Move command to the output queue and clear fields
    void _finalizeCommandParsing();

    //! Read 16bit unsigned integer. If byte array size isn't 2 return 0
    static uint16_t _readBigEndian(const QByteArray &data);

    //! Keeps number of data bytes left to read
    int _dataLeftSize = -1;

    //! Buffer to accumulated data bytes
    QByteArray _dataBuffer;

    //! Command being parsed
    std::shared_ptr<LanternCommand> _command;

    //! Parsed commands ready to be fetched
    std::queue<std::shared_ptr<LanternCommand>> _commandsParsed;
};
