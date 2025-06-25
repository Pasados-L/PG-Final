#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>

// Quita espacios al inicio y final
static inline std::string trim(const std::string& s) {
    auto ws = " \t\n\r";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

// Carga únicamente la sección [Bandera] en un map<clave, valor>
bool LoadBanderaInfo(const std::string& filename,
    std::map<std::string, std::string>& out) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    bool inSection = false;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (!inSection) {
            if (line == "[Bandera]") {
                inSection = true;
            }
            continue;
        }

        // dentro de sección
        if (line.front() == '[') {
            // nueva sección → dejamos de leer
            break;
        }

        // key=value
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim(line.substr(0, eq));
        std::string value = trim(line.substr(eq + 1));

        // reemplaza "\n" por salto real
        size_t pos = 0;
        while ((pos = value.find("\\n", pos)) != std::string::npos) {
            value.replace(pos, 2, "\n");
            pos += 1;
        }

        out[key] = value;
    }

    return true;
}
