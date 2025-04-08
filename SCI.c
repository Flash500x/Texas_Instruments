/*This codespace contains registers with SCI and HET registry declarations and initializations */
/* code simulated and written on Code Composer Studio by Texas Instruments 
for TMS570 series microcontrollers */
// author @Abin S Manoj
typedef unsigned int UInt32;

#define SCIPIO7     (*(volatile UInt32 *)0xFFF7E458UL)
#define SCIGCR1     (*(volatile UInt32 *)0xFFF7E404UL)
#define SCIPIO0     (*(volatile UInt32 *)0xFFF7E43CUL)
#define SCIFORMAT   (*(volatile UInt32 *)0xFFF7E428UL)
#define SCIBRS      (*(volatile UInt32 *)0xFFF7E42CUL)
#define SCIFLR      (*(volatile UInt32 *)0xFFF7E41CUL)
#define SCITD       (*(volatile UInt32 *)0xFFF7E438UL)
#define SCIRD       (*(volatile UInt32 *)0xFFF7E434UL)
#define SYS_CCR     (*(volatile UInt32 *)0xFFFFFFE0UL)

#define SCI_TXBUFFER_VACANT  0x00000100
#define SCI_RX_DATA_RECEIVED 0x00000200
#define SCI_TX_END           0x00000080

#define HETGCR      (*(volatile unsigned int *)0xFFF7B800UL)
#define HET_DIR     (*(volatile unsigned int *)0xFFF7B84CUL)
#define HET_DOUT    (*(volatile unsigned int *)0xFFF7B854UL)
#define HET_PDR     (*(volatile unsigned int *)0xFFF7B860UL)
#define HET_PULDIS  (*(volatile unsigned int *)0xFFF7B864UL)
#define HETPINDIS   (*(volatile unsigned int *)0xFFF7B894UL)

void intalize(void)
{
    HETGCR = 0x01000001; // Device normal working mode

    HET_PDR     = 0xFFFFFFC0; // open drain register disabled from 7–31
    HET_PULDIS  = 0xFFFFFFFF; // pull functionality disabled
    HETPINDIS   = 0x00000000; // all pins are configured as inputs
    HET_DOUT    = 0xFFFFFFFF; // all pins configured as outputs
    HET_DIR     = 0x0000003F; // config direction
    HET_DIR     = 0x0000003F; // 3rd pin activated

    // Pull Enabled for transmit, receive and SCICLK
    SCIPIO7 = 0x00000000;

    // Configures the LINTX pin as SCI/LIN TX and LINRX pin as SCI/LIN RX Pin
    SCIPIO0 = 0x00000006;

    // Loop back mode disabled, Debug mode affects transmission, Sleep mode is disabled,
    // Multi Buffer Mode is disabled, SCI Mode is enabled, Clk source is Internal Clk(SCICLK),
    // Parity disabled, one stop bit and asynchronous timing mode
    SCIGCR1 = 0x03000022;

    // The response field has 1 byte/Character and Character is 8 bits long
    SCIFORMAT = 0x00000007;

    // 19.2k baud (at 16MHz VBUSPCLK)
    SCIBRS = 0x0B000007;

    // Bring the SCI to ready state
    SCIGCR1 |= 0x80;
}

void sent_reg_message(unsigned char data_tx)
{
    SCITD = data_tx; // character to be sent loaded in the transmit data buffer register
    while (0 == (SCI_TX_END & SCIFLR))
    {
        // waiting for transmission to be complete
    }
}

void pointer_message_loader(unsigned char *pString)
{
    while (*pString)
        sent_reg_message(*(pString++));
}

int main()
{
    unsigned char kbr[4] = "Abin";
    unsigned char rxchar;

    intalize();
    int i = 0;
    int j = 0;

    while (1)
    {
        pointer_message_loader(kbr);
        HET_DIR = 0x04;
    }

    return 0;
}
