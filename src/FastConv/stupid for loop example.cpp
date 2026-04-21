for (int j = iLengthOfIr - 1; j >= 0; j--)
{
    if (i-j >= 0)
    {
        pfOutputBuffer[i] += m_pfImpulseResponse[j] * m_pfInputBuffer[i + j - iLengthOfIr];
    }
    
}