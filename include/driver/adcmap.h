/* 
adcmap.h
*/

// Prototype as C for weenies that want to use C++
#ifdef __cplusplus
extern "C" {
#endif

extern const ADCchannel_t ADCcmap_GCU_v303_full[];

void load_v303();
#ifdef __cplusplus
}
#endif
