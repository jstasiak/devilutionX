//HEADER_GOES_HERE
#ifndef __ENGINE_H__
#define __ENGINE_H__

//offset 0
//pCelBuff->pFrameTable[0]

extern char gbPixelCol; // automap pixel color 8-bit (palette entry)
extern BOOL gbRotateMap; // flip - if y < x
extern int orgseed;
extern int SeedCount;
extern BOOL gbNotInView; // valid - if x/y are in bounds

inline BYTE *CelGetFrameStart(BYTE *pCelBuff, int nCel)
{
	DWORD *pFrameTable;

	pFrameTable = (DWORD *)pCelBuff;

	return pCelBuff + SwapLE32(pFrameTable[nCel]);
}

inline int CelGetFrameSize(BYTE *pCelBuff, int nCel)
{
	DWORD *pFrameTable;

	pFrameTable = (DWORD *)pCelBuff;

	return SwapLE32(pFrameTable[nCel + 1]) - SwapLE32(pFrameTable[nCel]);
}

inline BYTE *CelGetFrame(BYTE *pCelBuff, int nCel, int *nDataSize)
{
	DWORD *pFrameTable;
	DWORD nCellStart;

	pFrameTable = (DWORD *)pCelBuff;
	nCellStart = SwapLE32(pFrameTable[nCel]);
	*nDataSize = SwapLE32(pFrameTable[nCel + 1]) - nCellStart;

	return pCelBuff + nCellStart;
}

inline BYTE *CelGetFrameClipped(BYTE *pCelBuff, int nCel, int CelSkip, int CelCap, int *nDataSize)
{
	BYTE *pRLEBytes;
	DWORD *pFrameTable;
	int nDataStart, nCellStart, nDataCap;

	pFrameTable = (DWORD *)pCelBuff;

	nCellStart = SwapLE32(pFrameTable[nCel]);
	pRLEBytes = &pCelBuff[nCellStart];
	nDataStart = SwapLE16(*(WORD *)&pRLEBytes[CelSkip]);
	if (nDataStart == 0)
		return NULL;

	*nDataSize = SwapLE32(pFrameTable[nCel + 1]) - nCellStart;
	if (CelCap == 8)
		nDataCap = 0;
	else
		nDataCap = SwapLE16(*(WORD *)&pRLEBytes[CelCap]);

	if (nDataCap)
		*nDataSize = nDataCap - nDataStart;
	else
		*nDataSize -= nDataStart;

	return pRLEBytes + nDataStart;
}

void CelDrawDatOnly(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void CelDecodeOnly(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth);
void CelDecDatOnly(BYTE *pBuff, BYTE *pCelBuff, int nCel, int nWidth);
void CelDrawHdrOnly(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void CelDecodeHdrOnly(BYTE *pBuff, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void CelDecDatLightOnly(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth, BYTE *tbl = NULL);
void CelDecDatLightTrans(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void CelDecodeLightOnly(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth);
void CelDecodeHdrLightOnly(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void CelDecodeHdrLightTrans(BYTE *pBuff, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void CelDrawHdrLightRed(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap, char light);
void Cel2DecDatOnly(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void Cel2DrawHdrOnly(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cel2DecodeHdrOnly(BYTE *pBuff, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cel2DecDatLightOnly(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void Cel2DecDatLightTrans(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void Cel2DecodeHdrLight(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cel2DecodeLightTrans(BYTE *pBuff, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cel2DrawHdrLightRed(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap, char light);
void CelDecodeRect(BYTE *pBuff, int always_0, int hgt, int wdt, BYTE *pCelBuff, int nCel, int nWidth);
void CelDecodeClr(char col, int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void CelDrawHdrClrHL(char col, int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void ENG_set_pixel(int sx, int sy, BYTE col);
void engine_draw_pixel(int sx, int sy);
void DrawLine(int x0, int y0, int x1, int y1, BYTE col);
int GetDirection(int x1, int y1, int x2, int y2);
void SetRndSeed(int s);
int GetRndSeed();
int random(BYTE idx, int v);
void engine_debug_trap(BOOL show_cursor);
BYTE *DiabloAllocPtr(DWORD dwBytes);
void mem_free_dbg(void *p);
BYTE *LoadFileInMem(char *pszName, DWORD *pdwFileLen);
DWORD LoadFileWithMem(const char *pszName, void *p);
void Cl2ApplyTrans(BYTE *p, BYTE *ttbl, int nCel);
void Cl2DecodeFrm1(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cl2DecDatFrm1(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void Cl2DecodeFrm2(char col, int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cl2DecDatFrm2(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth, char col);
void Cl2DecodeFrm3(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap, char light);
void Cl2DecDatLightTbl1(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth, BYTE *pTable);
void Cl2DecodeLightTbl(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cl2DecodeFrm4(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cl2DecDatFrm4(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth);
void Cl2DecodeClrHL(char col, int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void Cl2DecDatClrHL(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth, char col);
void Cl2DecodeFrm5(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap, char light);
void Cl2DecDatLightTbl2(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth, BYTE *pTable);
void Cl2DecodeFrm6(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth, int CelSkip, int CelCap);
void PlayInGameMovie(char *pszMovie);

void DrawSolidRectangle(int x0, int dx, int y0, int dy, int color);
int CalculateTextWidth(const char* s);

/* rdata */

extern const int RndInc;
extern const int RndMult;

#endif /* __ENGINE_H__ */
