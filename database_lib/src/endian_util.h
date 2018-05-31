#pragma once

inline uint32_t ntohl(uint32_t const net)
{
	uint8_t * p_net = (uint8_t *)&net;
	return ((uint32_t)p_net[0] << 24)
		| ((uint32_t)p_net[1] << 16)
		| ((uint32_t)p_net[2] << 8)
		| ((uint32_t)p_net[3] << 0);
}

inline void htonl(const float * const host_in, float * net)
{
	uint32_t * host = (uint32_t *)host_in;
	uint8_t * p_net = (uint8_t *)net;
	p_net[0] = (uint8_t)(*host >> 24);
	p_net[1] = (uint8_t)(*host >> 16);
	p_net[2] = (uint8_t)(*host >> 8);
	p_net[3] = (uint8_t)(*host >> 0);	
}

inline void htoni(const int * const host_in, int * net)
{
	uint32_t * host = (uint32_t *)host_in;
	uint8_t * p_net = (uint8_t *)net;
	p_net[0] = (uint8_t)(*host >> 24);
	p_net[1] = (uint8_t)(*host >> 16);
	p_net[2] = (uint8_t)(*host >> 8);
	p_net[3] = (uint8_t)(*host >> 0);
}

inline uint64_t ntohll(uint64_t const net)
{
	uint8_t * p_net = (uint8_t *)&net;
	return ((uint64_t)p_net[0] << 56)
		| ((uint64_t)p_net[1] << 48)
		| ((uint64_t)p_net[2] << 40)
		| ((uint64_t)p_net[3] << 32)
		| ((uint64_t)p_net[4] << 24)
		| ((uint64_t)p_net[5] << 16)
		| ((uint64_t)p_net[6] << 8)
		| ((uint64_t)p_net[7] << 0);
}

inline void htonll(const double * const host_in, double * net)
{	
	uint64_t * host = (uint64_t *) host_in;
	uint8_t * p_net = (uint8_t *) net;
	p_net[0] = (uint8_t)(*host >> 56);
	p_net[1] = (uint8_t)(*host >> 48);
	p_net[2] = (uint8_t)(*host >> 40);
	p_net[3] = (uint8_t)(*host >> 32);
	p_net[4] = (uint8_t)(*host >> 24);
	p_net[5] = (uint8_t)(*host >> 16);
	p_net[6] = (uint8_t)(*host >> 8);
	p_net[7] = (uint8_t)(*host >> 0);
}