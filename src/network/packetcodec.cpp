#include "packetcodec.h"
#include <QDebug>

PacketCodec::PacketCodec()
{
}

QByteArray PacketCodec::pack(const mechat::Packet &packet)//protobuf打包消息
{
    // 序列化 Protobuf 消息
    std::string body;
    if (!packet.SerializeToString(&body)) {//序列化Protobuf消息
        qWarning() << "Protobuf serialization failed";
        return QByteArray();
    }
    if (body.empty() || body.size() > MAX_PAYLOAD_SIZE) {
        qWarning() << "Protobuf payload is empty or exceeds the protocol limit";
        return QByteArray();
    }

    QByteArray bodyArray(body.data(), static_cast<int>(body.size()));//将Protobuf消息转换为QByteArray
    uint32_t payloadLen = static_cast<uint32_t>(bodyArray.size());//包体长度
    uint16_t crc = calculateCRC16(bodyArray);//计算CRC16

    // 组装数据包：MagicNum(2) + PayloadLen(4) + CRC16(2) + Body(N)
    QByteArray result;
    result.reserve(HEADER_SIZE + payloadLen);//预分配内存

    writeUint16(result, MAGIC_NUM);//魔数
    writeUint32(result, payloadLen);//包体长度
    writeUint16(result, crc);//CRC16校验码，处理文件是否完整
    result.append(bodyArray);//包体

    return result;
}

QByteArray PacketCodec::packRawData(const QByteArray &rawData)//打包原始数据
{
    uint32_t payloadLen = static_cast<uint32_t>(rawData.size());//包体长度
    uint16_t crc = calculateCRC16(rawData);//计算CRC16

    // 组装数据包：MagicNum(2) + PayloadLen(4) + CRC16(2) + Body(N)
    QByteArray result;
    result.reserve(HEADER_SIZE + payloadLen);//预分配内存

    writeUint16(result, MAGIC_NUM);//魔数
    writeUint32(result, payloadLen);//包体长度
    writeUint16(result, crc);//CRC16
    result.append(rawData);//原始数据

    return result;
}

QList<mechat::Packet> PacketCodec::unpack(const QByteArray &data)//protobuf解包消息
{
    QList<mechat::Packet> results;
    m_buffer.append(data);

    while (m_buffer.size() >= HEADER_SIZE) {
        // 读取魔数
        uint16_t magic = readUint16(m_buffer, 0);
        if (magic != MAGIC_NUM) {
            qWarning() << "Invalid magic number:" << magic;
            m_buffer.remove(0, 1);
            continue;
        }

        // 读取包体长度
        uint32_t payloadLen = readUint32(m_buffer, 2);
        if (payloadLen > MAX_PAYLOAD_SIZE) {
            qWarning() << "Payload too large:" << payloadLen;
            m_buffer.remove(0, 1);
            continue;
        }

        // 检查缓冲区是否包含完整包体
        int totalLen = HEADER_SIZE + payloadLen;
        if (m_buffer.size() < totalLen) {
            break; // 数据不足，等待更多数据
        }

        // 读取 CRC16
        uint16_t receivedCrc = readUint16(m_buffer, 6);

        // 提取 Body
        QByteArray body = m_buffer.mid(HEADER_SIZE, payloadLen);

        // 验证 CRC16
        uint16_t calculatedCrc = calculateCRC16(body);
        if (receivedCrc != calculatedCrc) {
            qWarning() << "CRC mismatch: received=" << receivedCrc
                       << "calculated=" << calculatedCrc;
            m_buffer.remove(0, totalLen);
            continue;
        }

        // 反序列化 Protobuf
        mechat::Packet packet;
        if (!packet.ParseFromArray(body.data(), body.size())) {
            qWarning() << "Protobuf deserialization failed";
            m_buffer.remove(0, totalLen);
            continue;
        }

        results.append(std::move(packet));
        m_buffer.remove(0, totalLen);
    }

    return results;
}

void PacketCodec::clearBuffer()//清空缓冲区
{
    m_buffer.clear();
}

QList<QByteArray> PacketCodec::unpackRaw(const QByteArray &data)//解包原始数据
{
    QList<QByteArray> results;
    m_buffer.append(data);

    while (m_buffer.size() >= HEADER_SIZE) {
        uint16_t magic = readUint16(m_buffer, 0);
        if (magic != MAGIC_NUM) {
            qWarning() << "Invalid magic number:" << magic;
            m_buffer.remove(0, 1);
            continue;
        }

        uint32_t payloadLen = readUint32(m_buffer, 2);
        if (payloadLen > MAX_PAYLOAD_SIZE) {
            qWarning() << "Payload too large:" << payloadLen;
            m_buffer.remove(0, 1);
            continue;
        }

        int totalLen = HEADER_SIZE + payloadLen;
        if (m_buffer.size() < totalLen) {
            break;
        }

        uint16_t receivedCrc = readUint16(m_buffer, 6);
        QByteArray body = m_buffer.mid(HEADER_SIZE, payloadLen);

        uint16_t calculatedCrc = calculateCRC16(body);
        if (receivedCrc != calculatedCrc) {
            qWarning() << "CRC mismatch: received=" << receivedCrc
                       << "calculated=" << calculatedCrc;
            m_buffer.remove(0, totalLen);
            continue;
        }

        results.append(body);
        m_buffer.remove(0, totalLen);
    }

    return results;
}

uint16_t PacketCodec::calculateCRC16(const QByteArray &data)//计算CRC16
{
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<uint8_t>(data[i]);
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

void PacketCodec::writeUint16(QByteArray &buffer, uint16_t value)//写入16位无符号整数
{
    buffer.append(static_cast<char>((value >> 8) & 0xFF));
    buffer.append(static_cast<char>(value & 0xFF));
}

void PacketCodec::writeUint32(QByteArray &buffer, uint32_t value)//写入32位无符号整数
{
    buffer.append(static_cast<char>((value >> 24) & 0xFF));
    buffer.append(static_cast<char>((value >> 16) & 0xFF));
    buffer.append(static_cast<char>((value >> 8) & 0xFF));
    buffer.append(static_cast<char>(value & 0xFF));
}

uint16_t PacketCodec::readUint16(const QByteArray &buffer, int offset)//读取16位无符号整数
{
    uint16_t value = 0;
    value |= (static_cast<uint8_t>(buffer[offset]) << 8);
    value |= static_cast<uint8_t>(buffer[offset + 1]);
    return value;
}

uint32_t PacketCodec::readUint32(const QByteArray &buffer, int offset)//读取32位无符号整数
{
    uint32_t value = 0;
    value |= (static_cast<uint32_t>(static_cast<uint8_t>(buffer[offset])) << 24);
    value |= (static_cast<uint32_t>(static_cast<uint8_t>(buffer[offset + 1])) << 16);
    value |= (static_cast<uint32_t>(static_cast<uint8_t>(buffer[offset + 2])) << 8);
    value |= static_cast<uint32_t>(static_cast<uint8_t>(buffer[offset + 3]));
    return value;
}
