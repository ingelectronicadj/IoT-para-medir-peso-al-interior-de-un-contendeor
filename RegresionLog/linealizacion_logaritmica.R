require(graphics)
pesos <- read.csv("~/pesos.csv")
ln_pesos<- log(pesos$peso)
ln2_pesos<- (log(pesos$peso))^2
vol_ln_pesos<- ln_pesos*pesos$difencial_potencial
dif_pot2<-pesos$difencial_potencial^2
a=(sum(vol_ln_pesos)-mean(pesos$difencial_potencial)*sum(ln_pesos))/(sum(ln2_pesos)-mean(ln_pesos)*sum(ln_pesos))
b=mean(pesos$difencial_potencial)-(a*mean(ln_pesos))
linealizada=function(x){a*log(x)+b}
ylineal<-linealizada(pesos$peso)
plot(pesos$peso,pesos$difencial_potencial,col="dark blue",type="o",lwd=3, main = "Pesos vs Diferencial de potencial",xlab="Peso(gr)",ylab="Diferencial de potencial(V)")
plot(pesos$peso,ylineal,col="dark red",type="o",lwd=3, main = "Pesos vs Diferencial de potencial",xlab="Peso(gr.)",ylab="Diferencial de potencial(V)")


pesos1_5k <- read.csv("~/pesos1_5k.csv")
ln_pesos2<- log(pesos1_5k$peso)
ln2_pesos<- (log(pesos1_5k$peso))^2
vol_ln_pesos2<- ln_pesos2*pesos1_5k$difencial_potencial
dif_pot2s<-pesos1_5k$difencial_potencial^2
a2=(sum(vol_ln_pesos2)-mean(pesos1_5k$difencial_potencial)*sum(ln_pesos2))/(sum(ln2_pesos)-mean(ln_pesos2)*sum(ln_pesos2))
b2=mean(pesos1_5k$difencial_potencial)-(a2*mean(ln_pesos2))
linealizada2=function(x){a2*log(x)+b2}
ylineal2<-linealizada2(pesos1_5k$peso)
plot(pesos1_5k$peso,pesos1_5k$difencial_potencial,col="dark blue",type="o",lwd=3, main = "Pesos vs Diferencial de potencial",xlab="Peso(gr)",ylab="Diferencial de potencial(V)")
plot(pesos1_5k$peso,ylineal2,col="dark red",type="o",lwd=3, main = "Pesos vs Diferencial de potencial",xlab="Peso(gr.)",ylab="Diferencial de potencial(V)")
