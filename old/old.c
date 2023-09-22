
void holdOn()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);
    APPEND_VAL(&hdr, CHOT_FUNCTION, F_HOLD_ON, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void holdOff()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);
    APPEND_VAL(&hdr, CHOT_FUNCTION, F_HOLD_OFF, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}
