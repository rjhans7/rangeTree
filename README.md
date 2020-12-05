# Range Tree

**Integrantes:**
- Piero Morales
- Roosevelt Ubaldo

## Ejecución

Para probar los experimentos que se encuentran en el archivo [experimentos.cpp](experimentos.cpp), se debe ejecutar el siguiente comando.

```
❯ g++ -std=c++17 -Wall source/experimentos.cpp
❯ ./a.out
```

Los resultados de las consultas realizadas durante la experimentación con 100k puntos insertados de manera aleatoria se encuentran en la carpeta [results](/results).

**Tiempo de ejecución de consultas:**

```
Build Range Tree of size: 100000 Time Taken: 0.930077
Query size: 10 Time Taken: 0.002214
Query size: 100 Time Taken: 0.069908
Query size: 1000 Time Taken: 0.644401
Query size: 10000 Time Taken: 7.06897
```

## Ejemplo de query

Un ejemplo de como se usan las queries se encuentra dentro del archivo [main.cpp](main.cpp)
![](query.PNG)
