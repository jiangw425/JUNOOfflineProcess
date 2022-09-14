unsigned int CdID2pmtID(unsigned int id)
{
	unsigned int pmtid = id - (0x10<<24);
	return (pmtid>>8);
}
