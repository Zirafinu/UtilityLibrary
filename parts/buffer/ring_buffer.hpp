
#ifndef UTILITY_LIBRARY_RING_BUFFER_HPP
#define UTILITY_LIBRARY_RING_BUFFER_HPP

template <typename BufferType>
class RingBuffer{
   public:
    explicit RingBuffer(BufferType&buffer):buffer(buffer){}


   private:
    BufferType& buffer;
};

#endif  // UTILITY_LIBRARY_RING_BUFFER_HPP
