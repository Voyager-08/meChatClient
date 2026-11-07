#ifndef PACKETCODEC_H
#define PACKETCODEC_H

#include <QByteArray>
#include <QList>
#include <cstdint>
#include "message.pb.h"

/**
 * @brief 二进制协议编解码器（Protobuf 版）
 * 
 * 协议格式：
 * +----------+------------+---------+---------+
 * | MagicNum | PayloadLen |  CRC16  |  Body   |
 * | 2 bytes  |  4 bytes   | 2 bytes | N bytes |
 * +----------+------------+---------+---------+
 * 
 * - MagicNum: 魔数 0xABCD，用于快速识别合法数据包，判断是不是我们的协议
 * - PayloadLen: Body 长度（4字节，大端序）
 * - CRC16: Body 的 CRC16 校验码（2字节），用于数据完整性校验
 * - Body: Protobuf 序列化的 Packet 数据
 */
class PacketCodec
{
public:
    static constexpr uint16_t MAGIC_NUM = 0xABCD;
    static constexpr int HEADER_SIZE = 8;
    static constexpr int MAX_PAYLOAD_SIZE = 10 * 1024 * 1024; // 10MB

    PacketCodec();

    // 打包 Protobuf Packet 为二进制数据包
    QByteArray pack(const mechat::Packet &packet);
    
    // 打包原始数据（用于文件传输等不需要 Protobuf 的场景）
    QByteArray packRawData(const QByteArray &rawData);

    // 向缓冲区追加数据，解包返回 Packet 列表
    QList<mechat::Packet> unpack(const QByteArray &data);
    
    // 向缓冲区追加数据，解包返回原始数据列表（用于文件传输）
    QList<QByteArray> unpackRaw(const QByteArray &data);

    void clearBuffer();

private:
    QByteArray m_buffer;

    static uint16_t calculateCRC16(const QByteArray &data);
    static void writeUint16(QByteArray &buffer, uint16_t value);
    static void writeUint32(QByteArray &buffer, uint32_t value);
    static uint16_t readUint16(const QByteArray &buffer, int offset);
    static uint32_t readUint32(const QByteArray &buffer, int offset);
};

#endif // PACKETCODEC_H
