# Packets 

### Typed or not?

- From one extreme to another? 
- How much can we use the type system? 
- Explosion of types?

```cpp
char packet[size];
...

template<typename Header, size_t PayloadSize, typename DataType>
struct Packet{
    Header header;
    DataType data[PayloadSize/sizeof(DataType)];
}

//Current implementation, doesn't hold any info
//on which type of data we have. Templated
//on header to (in theory) allow to work with other
//detectors
template<typename Header, size_t PayloadSize>
struct Packet{
    Header header;
    char data[PayloadSize];
}

```

### Building data structures on top

```cpp
//using std::vector?
std::vector<Packet<Header, PayloadSize>> packets;

//Even more templates?
RawFrameData<Packet<Header, PayloadSize>, NumPackets> raw_frame; 

//How much metadata? Owning non owning? Both? RawFrameView? 


```