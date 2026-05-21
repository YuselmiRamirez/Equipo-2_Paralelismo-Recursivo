# Paralelismo Recursivo
__*Equipo 2: Paralelismo recursivo con task*__  
Integrantes:  
- 321285535 *- Arceo Ramirez Ailyn Yuselmi* 
- 322024089 *- Grande Luna Samantha* 
- 322253700 *- Gutiérrez Martínez Esmeralda Rubí*

---

## Contexto previo.

__Divide y Vencerás.__  
Divide y vencerás se puede explicar en tres pasos, divide (divide), conquista (conquer) y fusiona (Merge). Divide separa el problema principal en subproblemas más pequeños, que representan una parte del problema general (versión más pequeña y manejable del problema original), hasta que ya no sea posible dividir. 
Para conquer, estos subproblemas se van resolviendo de forma individual, o a veces, cuando se nos presenta un caso base (subproblema muy pequeño) se resuelve directamente sin aplicar la recursión.
Al final, en merge, todos los subproblemas y sus soluciones se combinan recursivamente para tener una solución del problema completo

__Paralelismo.__  
El paralelismo es un paradigma en el que una tarea más grande se divide en sus tareas más pequeñas, independientes y procesadas simultáneamente. Su objetivo es acelerar nuestro programa al dividirlo en tareas ejecutables concurrentemente en diferentes procesadores o núcleos.
Esto se puede lograr con varios métodos, como los son los multihilo, con hilos creados y ejecutados con sus propios procesos en paralelo

__Hilos.__  
Los hilos, o también llamados subprocesos son una secuencia de instrucciones que el sistema puede programar para su ejecución. También llamados threads, es la unidad más pequeña de procesamiento, por lo que resultan fáciles de gestionar.
Estos no deben ser confundidos con procesos ya que los threads pertenecen a los procesos y, a su vez, un proceso puede tener múltiples threads. Estos se crean, ejecutan y finalizan dentro del proceso, es de aquí que el nombre subprocesos cobra sentido.

---

## Teoría.

A nivel de hardware y compilador, una task (tarea) no es un hilo. Es una estructura de datos abstracta que empaqueta dos cosas: un bloque de código que se debe ejecutar y su entorno de datos (es decir, copias de sus variables privadas y punteros a sus variables compartidas).

Tradicionalmente, con parallel for, OpenMP reparte iteraciones de forma estática o predecible. En cambio, las tareas permiten el paralelismo asíncrono. Cuando el programa encuentra la directiva #pragma omp task:

    1. El hilo actual no se detiene a ejecutarla; simplemente empaqueta la tarea.
    2. Coloca la tarea en una cola o estructura interna de almacenamiento de trabajo (el Task Pool).
    3. El hilo original continúa inmediatamente con las siguientes líneas de código, permitiendo un flujo dinámico ideal para algoritmos recursivos o estructuras irregulares (como árboles o grafos).

**¿Cómo se asigna dinámicamente a un hilo libre?**

Las implementaciones modernas de OpenMP no utilizan una sola cola global (ya que todos los hilos peleando por una sola fila causarían un cuello de botella por bloqueo). En su lugar, utilizan una arquitectura distribuida basada en dos conceptos:

* Colas Locales (LIFO): Cada hilo del procesador tiene su propia cola privada de tareas pendientes. Cuando un hilo genera una subtarea (por ejemplo, en una llamada recursiva), la mete a su propia cola y la procesa bajo el principio de LIFO. 
* Robo de Trabajo (Work-Stealing): Aquí ocurre una asignación dinámica, si un hilo termina todas las tareas de su propia fila y se queda libre, se activa un algoritmo "ladrón". Este hilo libre revisa de forma aleatoria las colas de los demás hilos y les "roba" dinámicamente tareas pendientes, garantiza un balanceo de carga automático y perfecto sin intervención de un hilo central.

**¿Por qué en problemas pequeños es más lento que el secuencial?**

El paralelismo no es gratis; la gestión de tareas introduce un costo oculto en el procesador conocido como Overhead (Sobrecarga). En problemas pequeños (por ejemplo, calcular un Fibonacci de un número bajo o procesar un arreglo de solo 10 elementos), el código paralelo colapsa el rendimiento por tres razones microarquitectónicas:

* Saturación de Memoria Dinámica: Crear una tarea requiere que el sistema operativo reserve memoria física para su entorno de datos. Si la recursión genera millones de micro-tareas en la base del árbol, el procesador gasta más tiempo reservando y liberando memoria en el sistema que ejecutando el código real.
* Destrucción de la Memoria Caché (Cache Misses): En la versión secuencial, los datos se quedan en la memoria caché ultra-rápida del núcleo. Pero si una micro-tarea es "robada" por un hilo de otro núcleo físico, el nuevo núcleo se ve obligado a buscar los datos desde la memoria RAM principal, destruyendo la localidad de datos y ralentizando el proceso.
* Costo de Sincronización: Monitorear el estado de las tareas y coordinar el freno de #pragma omp taskwait requiere operaciones atómicas a nivel de hardware. Si el trabajo de la tarea toma un microsegundo, pero coordinar la sincronización toma dos, el paralelismo se vuelve contraproducente.


---

## Analisis de resultados 
Primero que nada tenemos el código secuencial, al depender únicamente de stdio.h, ejecuta las llamadas de manera ordenada y consecutiva, lo que lo hace más simple pero lento en valores grandes de n, mientras que el código paralelo, al incluir omp.h, divide el cálculo en tareas que pueden ejecutarse al mismo tiempo en distintos hilos, logrando mayor eficiencia en números grandes aunque pero con sobrecarga en valores pequeños.En nuestro caso que tenemos el cálculo de Fibonacci, esto tiende a que las llamadas recursivas a fibonacci(n-1) y fibonacci(n-2) pueden ejecutarse simultáneamente, acelerando el cálculo, pero gracias a taskwait se garantiza que ambas finalicen antes de sumar sus resultados, aqui mismo podemos tener que taskwait va esperar a que se terminen las tareas; sin esta sincronización, el programa podría intentar usar valores incompletos o incorrectos. Recordando que no nos puede util para programas sencillos ya que tiene un mayor precio.

---

## Conclusiones.

---

## Referencias.
- GeeksforGeeks. (2026, 19 enero). Introduction to Divide and Conquer Algorithm. GeeksforGeeks. https://www.geeksforgeeks.org/dsa/introduction-to-divide-and-conquer-algorithm/ 
- Content Studio. (2024, 18 marzo). ¿Qué es el paralelismo de datos? Everpure. https://www.everpuredata.com/la/knowledge/what-is-data-parallelism.html
- Gálvez, J. A. S. (s. f.). Hilos. Unidades de Apoyo Para el Aprendizaje - CUAED - UNAM. https://repositorio-uapa.cuaed.unam.mx/repositorio/moodle/pluginfile.php/3084/mod_resource/content/1/UAPA-Hilos/index.html
- OpenMP Architecture Review Board. (2018). OpenMP Application Programming Interface Version 5.0. https://www.openmp.org
- Chandra, R., Dagum, L., Kohr, D., Maydan, D., McDonald, J., & Menon, R. (2001). Parallel programming in OpenMP. Morgan Kaufmann.