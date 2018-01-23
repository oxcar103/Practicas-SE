Practicas-SE  
========================

Prácticas de la asignatura Sistemas Empotrados (SE).  
------------------------

- **P1-Conexión y gestión remota de la plataforma**: En esta práctica, veremos cómo consultar los manuales para encontrar componentes en la placa, y cómo accederlos y grabarle una imagen en RAM a través de *openocd*.  

- **P2-Introducción al ensamblador**: En esta práctica, tendremos nuestra primera toma de contacto con la arquitectura ARM y su repertorio de instrucciones mediante la configuración de un programa básico en el cuál el led que parpadea depende del último botón pulsado.  

- **P3-Introducción al enlazador y el resto de las binutils**: En esta práctica, configuraremos el script de enlazado, estableciendo las direcciones de memoria que queremos usar y redistribuyendo las zonas de código en ella a nuestro gusto, y definiendo algunas constantes para el acceso rápido a las direcciones del GPIO. Además, modificaremos nuestro programa principal para que se aproveche de estos cambios.  

- **P4-Arranque e inicialización del entorno de ejecución**: En esta práctica, crearemos el *C RunTime* (**CRT**) con una configuración básica: inicializar los dispositivos del sistema(entrada, salida, excepciones,...), establecer los modos de ejecución de nuestra placa, las pilas necesarias y el heap. Además, pasaremos nuestro programa de prueba a C para comprobar que nuestra configuración funciona.  

- **P5-Gestión de excepciones e interrupciones**: En esta práctica, daremos nuestros primeros pasos en el manejo de excepciones así como en la deshabilitación/habilitación de interrupciones. Para ello, cambiaremos un manejador de excepciones y provocaremos dicha excepción; y haremos uso de una poderosa herramienta, el ensamblador en línea, para el cambio de los bits I y F directamente desde C e intentar así construir una región crítica.  

- **P6-El controlador de interrupciones**: En esta práctica, trabajaremos en el control de interrupciones modificando directamente los registros de la Econotag para ello, así como desarrollar más modos de deshabilitación/habilitacin de interrupciones. Para ello, cambiaremos un manejador de interrupciones, y habilitaremos y provocaremos dicha interrupción.  

- **P7-Gestión de los pines de entrada/salida**: En esta práctica, abstraeremos la capa hardware del GPIO mediante una API básica con una serie de funciones *get* y *set* para fijar ciertos pines a entrada/salida y con ello escribirlos, leerlos o asignarles una función usando el número de pin o mediante el puerto y una máscara si son del mismo puerto. Adicionalmente, modificaremos el programa que editamos por última vez en **P4** para adaptarlo a la abstracción actual de nuestro BSP.  

- **P8-Desarrollo de un driver L0 para las UART**: En esta práctica, empezaremos a desarrollar el diseño de los drivers, empezando por el nivel 0 o L0 consistente únicamente en la construcción de la estructura de registros, la inicialización correspondiente y las funciones de envío y recepción de caracteres. Adicionalmente, implementaremos un sencillo programa para testear el correcto funcionamiento de esta capa de drivers.  

- **P9-Desarrollo de un driver L1 para las UART**: En esta práctica, continuaremos con el desarrollo del diseño de los drivers, mediante la implementación del nivel 1 o L1 consistente principalmente en la incorporación del manejo de las interrupciones y de bufferes circulares para evitar el comportamiento bloqueante de las funciones de envío y recepción del L0. Adicionalmente, implementaremos un sencillo programa para testear el correcto funcionamiento de esta capa de drivers.  

- **P10-Desarrollo de un driver L2 para las UART**: En esta práctica, terminaremos poéticamente tanto con la asignatura como con  los drivers, culminando con el nivel 2 o L2 consistente esencialmente de la integración de nuestra placa con el estándar de C, permitiéndonos así la realización de cualquier aplicación para nuestro sistema empotrado de manera análoga a como lo haríamos para nuestro SO. Para esta integración, nos basta con la implementación mínima de 7 funciones stub: *_open*, *_close*, *_read*, *_write*, *_lseek*, *_fstat* e *_isatty* que sirven de base para las llamadas al sistema casi homónimas. Adicionalmente, traduciremos el programa de la práctica anterior plenamente a funciones estándar de C para testear el correcto funcionamiento de esta capa de drivers.  

Licencia  
------------------------
El proyecto en conjunto se distribuye bajo la licencia [GPL](LICENSE).  

Sin embargo, la licencia no se aplicará sobre el material aportado por el profesor.  

