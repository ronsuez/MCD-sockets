void abrir_archivo(char * ruta){
    
    FILE *f = fopen(ruta, "w");
    
    if (f==NULL){
        perror ("Error al abrir %s", ruta);
        return -1;
        }
};
void leer_archivo(char * ruta);