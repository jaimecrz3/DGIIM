#include <stdio.h>

int main(void)
{
{
	int n;
	int curr;
	int d;
	int ultim;
	int cuenta;
	int primero;
	printf("%s", "introduce numero : ");
	scanf("%d", &n);
	printf("%d", n);
	printf("%s", " = ");
	curr = 2;
	ultim = 0;
	cuenta = 0;
	primero = 1;
etiqueta10:
	int temp0;
	temp0 = curr <= n;
	if (!temp0) goto etiqueta11;
{
	int temp1;
	temp1 = n / curr;
	d = temp1;
	int temp2;
	temp2 = d * curr;
	int temp3;
	temp3 = (temp2 == n);
	if (!temp3) goto etiqueta8;
{
	int temp4;
	temp4 = (curr != ultim);
	if (!temp4) goto etiqueta0;
{
	ultim = curr;
	cuenta = 1;
}
	goto etiqueta1;
etiqueta0:
{
	int temp5;
	temp5 = cuenta + 1;
	cuenta = temp5;
}
etiqueta1:
	int temp6;
	temp6 = n / curr;
	n = temp6;
}
	goto etiqueta9;
etiqueta8:
{
	int temp7;
	temp7 = cuenta > 0;
	if (!temp7) goto etiqueta6;
{
	int temp8;
	temp8 = (primero == 0);
	if (!temp8) goto etiqueta2;
{
	printf("%s", " *");
}
	goto etiqueta3;
etiqueta2:
etiqueta3:
	primero = 0;
	printf("%s", " ");
	printf("%d", curr);
	int temp9;
	temp9 = cuenta > 1;
	if (!temp9) goto etiqueta4;
{
	printf("%s", "^");
	printf("%d", cuenta);
}
	goto etiqueta5;
etiqueta4:
etiqueta5:
}
	goto etiqueta7;
etiqueta6:
etiqueta7:
	int temp10;
	temp10 = curr + 1;
	curr = temp10;
	cuenta = 0;
}
etiqueta9:
}
	goto etiqueta10;
etiqueta11:
	int temp11;
	temp11 = cuenta > 0;
	if (!temp11) goto etiqueta16;
{
	int temp12;
	temp12 = (primero == 0);
	if (!temp12) goto etiqueta12;
{
	printf("%s", " *");
}
	goto etiqueta13;
etiqueta12:
etiqueta13:
	primero = 0;
	printf("%s", " ");
	printf("%d", curr);
	int temp13;
	temp13 = cuenta > 1;
	if (!temp13) goto etiqueta14;
{
	printf("%s", "^");
	printf("%d", cuenta);
}
	goto etiqueta15;
etiqueta14:
etiqueta15:
}
	goto etiqueta17;
etiqueta16:
etiqueta17:
	printf("%s", "\n");
}
	return 0;
}
