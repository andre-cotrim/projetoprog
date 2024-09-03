
# Programming project

## Group elements

Identify all group elements (numbers and names).

-up202305592 André Cotrim
-up202303793 Beatriz Bastos
-up202306568 Joana Carvalhal

## Accomplished tasks

Neste projeto implementamos uma conversão do formato SVG vetorial para o formato PNG raster. De facto, obtivemos esta implementação através de várias tarefas: consideração de elementos geométricos, transformação de elementos, grupos de elementos e duplicação de elementos. Inserido nos elementos geométricos a considerar temos: "ellipse", "circle", "polyline", "line", "polygon" e "rect". Na verdade, cada elemento tem várias funções associadas de maneira a ser possível fazer as suas transformações. Nessas funções temos incluídas "draw", "translate", "rotate", "scale" e "duplicate", das quais apenas "translate", "rotate" e "scale" são transformações.  Posteriormente, de modo a reconhecer o elemento que se deseja desenhar, criamos a função "parse_elements" e, para identificar a transformação a ser aplicada, se assinalado, construimos a função "transform". Além disso, nos grupos temos embutidas funções que transformam e desenham grupos de elementos geométricos. Efetivamente, para transformar os elementos inseridos num grupo, armazenamo-los num vetor. Esse vetor é depois percorrido iterativamente, descobrindo de que elemento se trata e que transformação se deve utilizar, caso aplicável, desenhando o grupo elemento a elemento. Por fim, para utilizar a função "duplicate" é encontrado o "id" do elemento e, caso lhe seja correspondido um "href", realizar-se-á a sua duplicação, aplicando as devidas transformações, se indicado. 