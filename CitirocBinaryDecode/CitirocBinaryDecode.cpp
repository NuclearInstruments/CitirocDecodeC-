// CitirocBinaryDecode.cpp : Defines the entry point for the console application.
//
#define FW_USE_VALIDATION 0

#ifdef _WIN32
#include "stdafx.h"
#endif
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string> 
#include <iomanip> 
using namespace std;
int main()
{
	//Input Data file (binary)
	std::ifstream file("E:/GIT/DT5550W-CITIROC/DecodeBinary/CitirocBinaryDecode/run_14.data", std::ios::binary | std::ios::ate);
	//output Data file (text)
	std::ofstream file0("E:/GIT/DT5550W-CITIROC/DecodeBinary/CitirocBinaryDecode/run_14_0.csv", std::ios::binary | std::ios::ate);

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		//Check if we have data
		if (!buffer.empty()) {
			uint32_t p = 0;
			uint32_t t = 0;
			uint32_t s = 0;
			uint32_t bword = 0;
			uint16_t AsicID;
			uint64_t EventTimecode;
			uint64_t RunEventTimecode;
			uint64_t EventCounter;
			uint32_t * datarow = new uint32_t[97];
			uint32_t *chargeHG = new uint32_t[32];
			uint32_t *chargeLG = new uint32_t[32];
			uint32_t TriggerID = 0;
			uint32_t ValidationID = 0;
			uint32_t Flags = 0;
			uint32_t ThresholdSoftware = 1;
			uint32_t DecodedPackets=0;
			bool * hit = new bool[32];

			//Convert vector in uin32
			const uint32_t s_32 = size/4;
			const uint32_t *p_ui32 = (uint32_t*)&(buffer[0]);

			//Decode vector
			while (p < s_32)
			{
				switch (s)
				{
				case 0:
					bword = p_ui32[p++];
					//Align to sync world
					if (((bword >> 4) & 0xc000000) == 0x8000000)
					{
						s = 1;
						AsicID = bword & 0xF;
						EventTimecode = (uint64_t) (p_ui32[p++]);
						RunEventTimecode = ((uint64_t)(p_ui32[p++]));
						RunEventTimecode += ((uint64_t)(p_ui32[p++])) << 32L;
						EventCounter = (uint64_t)(p_ui32[p++]);

						t = t + 5;
					}
					else
						t++;
					break;

				case 1:
					for (int i = 0; i < 32; i++)
					{
						bword = p_ui32[p++];
						datarow[i * 3 + 0] = (bword >> 0) & 0x3FFF;
						datarow[i * 3 + 1] = (bword >> 14) & 0x3FFF;
						datarow[i * 3 + 2] = (bword >> 28) & 0x1;
						t++;
					}
					for (int i = 0; i < 32; i++)
					{
						hit[31 - i] = (bool)((datarow[i * 3 + 2] & 0x1) == 1 ? true : false);
						int dataHG = (int)datarow[(i * 3) + 0];
						int dataLG = (int)datarow[(i * 3) + 1];


						if (dataHG > ThresholdSoftware)
							chargeHG[31 - i] = (dataHG);
						else
							chargeHG[31 - i] = 0;


						if (dataHG > ThresholdSoftware)
							chargeLG[31 - i] = dataLG;
						else
							chargeLG[31 - i] = 0;

					}
						
					if (FW_USE_VALIDATION == 1)
						s = 2;
					else
						s = 5;
						
					break;
				case 2:
					TriggerID = p_ui32[p++];	
					s = 3;
					break;
				case 3:
					ValidationID = p_ui32[p++];	
					s = 4;
					break;		
				case 4:
					Flags = p_ui32[p++];	
					s = 5;
					break;	
						
				case 5:
					if ((p_ui32[p++] & 0xc0000000) == 0xc0000000)
					{
						//Save information on file in a custom format
						file0 << std::setw(16) << DecodedPackets << "\t" << std::setw(4) << AsicID << "\t";
						file0 << std::setw(16) << EventTimecode << "\t";
						file0 << std::setw(20) << RunEventTimecode << "\t";
						file0 << std::setw(16) << EventCounter << "\t";
						if (FW_USE_VALIDATION == 1) {
							file0 << std::setw(20) << TriggerID << "\t";
							file0 << std::setw(20) << ValidationID << "\t";
							file0 << std::setw(20) << Flags << "\t";
						}
						for (int i = 0; i < 32; i++)
						{
							file0 << std::setw(4) << std::to_string(hit[i]) << (i == 31 ? "" : "\t");
						}

						for (int i = 0; i < 32; i++)
						{
							file0 << std::setw(8) << std::to_string(chargeLG[i]) << "\t";
						}

						for (int i = 0; i < 32; i++)
						{
							file0 << std::setw(8) << std::to_string(chargeHG[i]) << (i == 31 ? "" : "\t");
						}
						file0 << endl;
					
					
						DecodedPackets++;
					}
					t++;
					s = 0;
					break;
				}
			}
		}
	}
    return 0;
}

