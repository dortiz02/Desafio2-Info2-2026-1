#include "../include/Simulador.h"
#include <random>
#include <cmath>

// Motor y distribucion estaticos — se inicializan una sola vez
// usando random_device como semilla, que es C++ puro
static std::mt19937                          motor(std::random_device{}());
static std::uniform_real_distribution<double> distrib(0.0, 1.0);

Simulador::Simulador() : alpha(0.6), beta(0.4), mu(1.35) {}
Simulador::Simulador(const Simulador& otro) : alpha(otro.alpha), beta(otro.beta), mu(otro.mu) {}
Simulador::~Simulador() {}

/// Genera numero aleatorio uniforme en [0.0, 1.0)
double Simulador::randProb() const {
    return distrib(motor);
}

/// Calcula goles esperados del equipo A contra B
/// Formula del enunciado:
double Simulador::calcularLambda(const Equipo& a, const Equipo& b) const {
    double gfa = a.getPromGF();
    double gcb = b.getPromGC();
    return mu * std::pow(gfa / mu, alpha) * std::pow(gcb / mu, beta);
}

/// Posesion proporcional inversa al ranking FIFA
/// Menor ranking (mejor equipo) implica mayor posesion
double Simulador::calcularPosesion(int rankingA, int rankingB) const {
    double pesoA = 1.0 / rankingA;
    double pesoB = 1.0 / rankingB;
    return (pesoA / (pesoA + pesoB)) * 100.0;
}

/// Selecciona 11 indices unicos aleatorios de la plantilla de 26
/// Usa Fisher-Yates parcial — solo necesita los primeros 11
/// El parametro e se reserva para extensiones futuras (filtrar lesionados, etc)
Lista<int> Simulador::elegirConvocados(const Equipo& /*e*/) const {
    int disponibles[26];
    for (int i = 0; i < 26; i++) disponibles[i] = i;
    Lista<int> sel;
    std::uniform_int_distribution<int> distInt(0, 25);
    for (int i = 0; i < 11; i++) {
        std::uniform_int_distribution<int> rango(i, 25);
        int idx = rango(motor);
        int tmp = disponibles[i]; disponibles[i] = disponibles[idx]; disponibles[idx] = tmp;
        sel.agregar(disponibles[i]);
    }
    return sel;
}

/// Simula las metricas de un jugador en el partido
/// Las probabilidades son estaticas por jugador — independientes de si ya anoto
void Simulador::simularJugador(ActaJugador& aj, int& golesRestantes) const {
    // Goles: 4% por jugador hasta agotar los goles esperados del equipo
    if (golesRestantes > 0 && randProb() < 0.04) {
        aj.setGoles(aj.getGoles() + 1);
        golesRestantes--;
    }
    // Amarillas: 6% primera, 1.15% segunda
    int amarillas = 0;
    if (randProb() < 0.06) { amarillas++; if (randProb() < 0.0115) amarillas++; }
    aj.setTarjetasAmarillas(amarillas);
    // Roja: solo si acumula dos amarillas
    aj.setTarjetasRojas(amarillas >= 2 ? 1 : 0);
    // Faltas: 13% primera, 2.75% segunda, 0.7% tercera
    int faltas = 0;
    if (randProb() < 0.13) { faltas++; if (randProb() < 0.0275) { faltas++; if (randProb() < 0.007) faltas++; } }
    aj.setFaltas(faltas);
}

/// Simula el partido completo — llena las dos actas
/// Si prorroga=true, los minutos se contabilizan en 120
void Simulador::simularPartido(Partido& p, bool prorroga) {
    int minutos   = prorroga ? 120 : 90;
    Equipo* local = p.getActaLocal().getEquipo();
    Equipo* visit = p.getActaVisitante().getEquipo();

    double lambdaL = calcularLambda(*local, *visit);
    double lambdaV = calcularLambda(*visit, *local);
    int golesL = static_cast<int>(std::round(lambdaL));
    int golesV = static_cast<int>(std::round(lambdaV));

    Lista<int> idxL = elegirConvocados(*local);
    Lista<int> idxV = elegirConvocados(*visit);

    for (int i = 0; i < 11; i++) {
        ActaJugador aj(&local->getPlantilla()[idxL[i]]);
        aj.setMinutosJugados(minutos);
        simularJugador(aj, golesL);
        p.getActaLocal().agregarConvocado(aj);
    }
    for (int i = 0; i < 11; i++) {
        ActaJugador aj(&visit->getPlantilla()[idxV[i]]);
        aj.setMinutosJugados(minutos);
        simularJugador(aj, golesV);
        p.getActaVisitante().agregarConvocado(aj);
    }

    // Goles restantes que no se asignaron a jugadores van al ultimo convocado
    if (golesL > 0) p.getActaLocal().getConvocados()[10].setGoles(
        p.getActaLocal().getConvocados()[10].getGoles() + golesL);
    if (golesV > 0) p.getActaVisitante().getConvocados()[10].setGoles(
        p.getActaVisitante().getConvocados()[10].getGoles() + golesV);

    // Calcular totales reales de goles
    int totalL = 0, totalV = 0;
    for (int i = 0; i < 11; i++) {
        totalL += p.getActaLocal().getConvocados()[i].getGoles();
        totalV += p.getActaVisitante().getConvocados()[i].getGoles();
    }
    p.getActaLocal().setGolesAFavor(totalL);
    p.getActaLocal().setGolesEnContra(totalV);
    p.getActaVisitante().setGolesAFavor(totalV);
    p.getActaVisitante().setGolesEnContra(totalL);

    // Posesion proporcional al ranking
    double posL = calcularPosesion(local->getRankingFIFA(), visit->getRankingFIFA());
    p.getActaLocal().setPosesion(posL);
    p.getActaVisitante().setPosesion(100.0 - posL);
    p.setResueltoEnProrroga(prorroga);
}

/// Desempate por ranking — sesgo proporcional inverso al ranking FIFA
/// Devuelve 0 si gana local, 1 si gana visitante
int Simulador::romperEmpate(const Equipo& local, const Equipo& visitante) const {
    double pesoL = 1.0 / local.getRankingFIFA();
    double pesoV = 1.0 / visitante.getRankingFIFA();
    return (randProb() < pesoL / (pesoL + pesoV)) ? 0 : 1;
}

std::ostream& operator<<(std::ostream& os, const Simulador& s) {
    os << "Simulador [alpha=" << s.alpha << " beta=" << s.beta << " mu=" << s.mu << "]";
    return os;
}
