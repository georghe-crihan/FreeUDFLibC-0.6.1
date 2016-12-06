#ifndef PPK_IB_ACCESS_INCLUDED
#define PPK_IB_ACCESS_INCLUDED

#ifdef __cplusplus
 extern "C"
 {
#endif
int compteconnexions(int *);
int essaiconnexion(int *, int *, char *);
int finconnexion(int *, int *);
int forcerecriture(char *);
int debutecriture(void);
int finecriture(void);
#ifdef __cplusplus
 }
#endif
#endif /* PPK_IB_ACCESS_INCLUDED */
