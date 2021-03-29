/* POPEN.C: This program uses _popen and _pclose to receive a
 * stream of text from a system process.
 */

#include <stdio.h>
#include <stdlib.h>

void main(void)
{

    char   psBuffer[128];
    FILE* chkdsk;

    /* Run DIR so that it writes its output to a pipe. Open this
       * pipe with read text attribute so that we can read it
     * like a text file.
       */
    if ((chkdsk = _popen("dir", "rt")) == NULL)
        exit(1);

    /* Read pipe until end of file. End of file indicates that
     * CHKDSK closed its standard out (probably meaning it
   * terminated).
     */
    while (!feof(chkdsk))
    {
        if (fgets(psBuffer, 128, chkdsk) != NULL)
            printf(psBuffer);
    }

    /* Close pipe and print return value of CHKDSK. */
    printf("\nProcess returned %d\n", _pclose(chkdsk));
}