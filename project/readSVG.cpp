#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <vector>
#include <sstream>
#include <map>
using namespace std;
using namespace tinyxml2;

namespace svg
{
    //! Normalisar pontos usados no vetor "points" em "Polyline" e "Polygon"
    //! @param elem elementos dos ficheiro svg
    //! @return vetor de pontos normalizados
    vector<Point> check_points(XMLElement *elem) {
        string pointsstring = elem -> FindAttribute("points") -> Value();
        vector<Point> points;
        stringstream ss(pointsstring);
        char delimiter;
        Point point;      
        while (ss >> point.x >> delimiter >> point.y) { //! Lê os valores da string e armazena-os no vetor 
            points.push_back(point);
            ss.ignore(); //! Se quiser ignorar a vírgula
        }
        return points;
    }

    //! Função "transform" usando "templates" para uma única função conseguir atuar sobre todos os objetos, independemente do seu tipo
    //! @param object objeto sobre o qual a transformação irá atuar
    //! @param elem elementos dos ficheiro svg para aceder a atributos
    template <typename T>
    void transform(T *object, XMLElement *elem) {
        if (elem -> FindAttribute("transform")) {  //! Se tiver o atributo "transform", armazena-o numa string
            string transform = elem -> FindAttribute("transform") -> Value();
            for (char& c : transform) { //! Troca vírgulas por espaços valor do atributo tranform para facilitar processos seguintes
                if (c == ',') {
                    c = ' ';
                }
            }
            Point origin_transform;
            if (elem -> FindAttribute("transform-origin")) { //! Caso contenha o atributo "transform-origin", armazena a origem num ponto com os seu valor
                string origin =  elem -> FindAttribute("transform-origin") -> Value();
                int space_ = origin.find(" ") ;
                int end_ = origin.find(")",space_);
                origin_transform.x = stoi(origin.substr(0, space_));
                origin_transform.y = stoi(origin.substr(space_, end_-space_));
            } else { //! Em caso contrário, armazena-o com {0,0}
                origin_transform = {0,0};
            }
            if (transform.find("translate") != std::string::npos) { //! Caso o "transform" seja "translate", efetua-o no elemento com o ponto adquirido no atributo
                int space = transform.find(" ");
                int end = transform.find(")");
                Point t;
                t.x = stoi(transform.substr(10, space-10));
                t.y = stoi(transform.substr(space, end-space)); 
                object -> translate(t);
            }  else if (transform.find("rotate") != string::npos) { //! Caso o "transform" seja "rotate", efetua-o no elemento com o ângulo adquirido no atributo e a origem
                string n = "";
                int angle;
                for (char c : transform) {
                    if (isdigit(c) || c == '-') {
                        n += c;
                    }
                }
                angle = stoi(n);
                object -> rotate(origin_transform, angle);
            } else if (transform.find("scale") != string::npos) {  //! Caso o "transform" seja "scale", efetua-o no elemento com o fator adquirido no atributo e a origem
                string n = "";
                int v;
                for (char c : transform) {
                    if (isdigit(c)) {
                        n += c;
                    }
                }
                v = stoi(n);
                object -> scale(origin_transform, v);
            }
        }
    }

    //! Verificar que elementos estão contidos no vetor iterativamente, colocando-os no "svg_elements" para posterior desenhar corretamente
    //! Caso o elemento seja "g" (groups), criamos um novo vetor de pontos para correr esta mesma função recursivamente e armazenar lá os seus elementos
    //! @param name nome do elemento para comparação e criação de objeto
    //! @param svg_elements vetor onde se armazenam os elementos para converter para PNG posteriormente
    //! @param elem elementos do ficheiro svg
    //! @param ids atributo para duplicação caso o elemento seja um "use" 
    void parse_elements(string name, vector<SVGElement *>& svg_elements, XMLElement *elem, map<string,SVGElement*> &ids) {
        string id; 
        if (name == "ellipse") {  
            Color fill = parse_color(elem -> FindAttribute("fill") -> Value());
            Point center = {elem -> IntAttribute("cx"), elem -> IntAttribute("cy")};
            Point radius = {elem -> IntAttribute("rx"), elem -> IntAttribute("ry")};
            Ellipse *object = new Ellipse(fill, center, radius); 
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            } 
            svg_elements.push_back(object);
        } else if (name == "circle") {
            Color fill = parse_color(elem -> FindAttribute("fill") -> Value());
            Point center = {elem -> IntAttribute("cx"), elem -> IntAttribute("cy")};
            Point radius = {elem -> IntAttribute("r"), elem -> IntAttribute("r")};
            Circle *object = new Circle(fill, center, radius);
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            }  
            svg_elements.push_back(object);
        } else if (name =="line") {
            Color stroke = parse_color(elem -> FindAttribute("stroke") -> Value());
            Point p1 = {elem -> IntAttribute("x1"), elem -> IntAttribute("y1")};
            Point p2 = {elem -> IntAttribute("x2"), elem -> IntAttribute("y2")};
            Line *object = new Line(stroke, p1, p2);
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            } 
            svg_elements.push_back(object);
        } else if (name =="polyline") {
            vector<Point> points_polyline = check_points(elem);
            Color stroke = parse_color(elem -> FindAttribute("stroke") -> Value());
            Polyline *object = new Polyline(stroke, points_polyline);
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            } 
            svg_elements.push_back(object);  
        } else if (name =="polygon") {
            vector<Point> pontos_polygon = check_points(elem);
            Polygon *object = new Polygon(parse_color(elem -> FindAttribute("fill") -> Value()), pontos_polygon);
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            } 
            svg_elements.push_back(object);
        } else if (name == "rect") {
            int x = elem -> IntAttribute("x");
            int y = elem -> IntAttribute("y");
            int height = elem -> IntAttribute("height");
            int width = elem -> IntAttribute("width");
            Color fill = parse_color(elem -> FindAttribute("fill") -> Value());
            vector<Point> vector_points_ = {{x,y},{x+width-1,y},{x+width-1,y+height-1},{x,y+height-1}}; //! Outros pontos do retângulo 
            Rect *object = new Rect(fill, vector_points_, width, height);
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            } 
            svg_elements.push_back(object);
        } else if (name =="g") {
            vector<SVGElement*> elements;
            for (XMLElement *elemento_ = elem -> FirstChildElement(); elemento_ != nullptr; elemento_= elemento_ -> NextSiblingElement()) { //! Armazenar os elementos do grupo num vetor recursivamente
                string name_ = elemento_ -> Name();
                parse_elements(name_, elements, elemento_, ids);
            }
            Group *object = new Group(elements);
            transform(object, elem);
            if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                id = elem -> FindAttribute("id") -> Value();
                ids.insert({id, object});
            }  
            svg_elements.push_back(object);
        } else if (name == "use") {
            string href = elem -> FindAttribute("href") -> Value();
            string id_ = "";
            for (char c : href) { //! Eliminar "#" do "href" para comparação com o "id"
                if (c != '#') {
                    id_ += c;
                }
            } 
            auto it = ids.find(id_); //! Procurar "id_" no vetor "ids"
            if (it != ids.end()) { //! Duplicar e transformar o elemento encontrado
                SVGElement* dup_elem = it -> second -> duplicate();
                transform(dup_elem, elem);
                svg_elements.push_back(dup_elem);
                if (elem -> FindAttribute("id")) { //! Inserir o "id" do elemento e o respetivo elemento no vetor "ids" para a duplicação
                    id = elem -> FindAttribute("id") -> Value();
                    ids.insert({id, dup_elem}); 
                }  
            } 
        }
    }

    //! Função base onde se acede à resolução da imagem para a converter em PNG e se acede a cada elemento do vetor
    //! @param svg_file ficheiro svg 
    //! @param dimensions dimensões para criar a imagem PNG
    //! @param svg_elements vetor onde se armazena os elementos para converter para PNG posteriormente
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS) {
            throw runtime_error("Unable to load " + svg_file);
        }
        
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem -> IntAttribute("width");
        dimensions.y = xml_elem -> IntAttribute("height");

        map<string,SVGElement*> ids;
        for (XMLElement *elem = xml_elem->FirstChildElement(); elem != nullptr; elem = elem -> NextSiblingElement()) { 
            string name = elem -> Name();
            parse_elements(name, svg_elements, elem, ids);
        }
    }
}