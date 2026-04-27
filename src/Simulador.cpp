#include "../include/Simulador.h"
#include <cstdlib>
#include <cmath>

// ── Constructores ─────────────────────────────────────────────

Simulador::Simulador() : alpha(0.6), beta(0.4), mu(1.35) {}

Simulador::Simulador(const Simulador& otro)
    : alpha(otro.alpha), beta(otro.beta), mu(otro.mu) {}

Simulador::~Simulador() {}

// ── Helpers ───────────────────────────────────────────────────

// Num aleatorio uniforme [0, 1)
double Simulador::randProb() const {
    return static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX) + 1.0);
}

// ── Logica de simulacion ──────────────────────────────────────

// Formula del enunciado:
// λA = µ * (GFA/µ)^α * (GCB/µ)^β
double Simulador::calcularLambda(const Equipo& a, const Equipo& b) const {
    double gfa = a.getPromGF();
    double gcb = b.getPromGC();
    return mu * std::pow(gfa / mu, alpha) * std::pow(gcb / mu, beta);
}

// Posesion proporcional inversa al ranking
// equipo con menor ranking (mejor) tiene mas posesión
// Mismo modelo que el desempate por sesgo aleatorio
double Simulador::calcularPosesion(int rankingA, int rankingB) const {
    // Invertir ranking: mejor equipo (ranking más bajo) → número mayor
    double pesoA = 1.0 / rankingA;
    double pesoB = 1.0 / rankingB;
    return (pesoA / (pesoA + pesoB)) * 100.0;
}

// Elige 11 indices únicos aleatorios de la plantilla de 26
Lista<int> Simulador::elegirConvocados(const Equipo& e) const {
    int disponibles[26];
    for (int i = 0; i < 26; i++) disponibles[i] = i;

    // Fisher-Yates shuffle parcial — solo necesitamos 11
    Lista<int> seleccionados;
    int tam = 26;
    for (int i = 0; i < 11; i++) {
        int idx = i + std::rand() % (tam - i);
        // intercambiar
        int tmp          = disponibles[i];
        disponibles[i]   = disponibles[idx];
        disponibles[idx] = tmp;
        seleccionados.agregar(disponibles[i]);
    }
    return seleccionados;
}

// Simula las metricas de un jugador en el partido
// golesRestantes se decrementa cuando el jugador marca
void Simulador::simularJugador(ActaJugador& aj, int& golesRestantes) const {
    // Goles — prob 4% por jugador, hasta agotar los goles esperados
    if (golesRestantes > 0 && randProb() < 0.04) {
        aj.setGoles(aj.getGoles() + 1);
        golesRestantes--;
    }

    // Tarjetas amarillas — 6% primera, 1.15% segunda
    int amarillas = 0;
    if (randProb() < 0.06) {
        amarillas++;
        if (randProb() < 0.0115) amarillas++;
    }
    aj.setTarjetasAmarillas(amarillas);

    // Roja — solo si tiene dos amarillas
    aj.setTarjetasRojas(amarillas >= 2 ? 1 : 0);

    // Faltas — 13% primera, 2.75% segunda, 0.7% tercera
    int faltas = 0;
    if (randProb() < 0.13) {
        faltas++;
        if (randProb() < 0.0275) {
            faltas++;
            if (randProb() < 0.007) faltas++;
        }
    }
    aj.setFaltas(faltas);
}

// Simula el partido completo
void Simulador::simularPartido(Partido& p, bool prorroga) {
    int minutos = prorroga ? 120 : 90;

    Equipo* local    = p.getActaLocal().getEquipo();
    Equipo* visitante = p.getActaVisitante().getEquipo();

    // Calcular goles esperados para cada equipo
    double lambdaLocal    = calcularLambda(*local, *visitante);
    double lambdaVisitante = calcularLambda(*visitante, *local);

    // Redondear a enteros para la simulación
    int golesLocal    = static_cast<int>(std::round(lambdaLocal));
    int golesVisitante = static_cast<int>(std::round(lambdaVisitante));

    // Elegir convocados
    Lista<int> idxLocal    = elegirConvocados(*local);
    Lista<int> idxVisitante = elegirConvocados(*visitante);

    // Crear actas de jugadores y simular
    for (int i = 0; i < 11; i++) {
        ActaJugador aj(&local->getPlantilla()[idxLocal[i]]);
        aj.setMinutosJugados(minutos);
        simularJugador(aj, golesLocal);
        p.getActaLocal().agregarConvocado(aj);
    }

    for (int i = 0; i < 11; i++) {
        ActaJugador aj(&visitante->getPlantilla()[idxVisitante[i]]);
        aj.setMinutosJugados(minutos);
        simularJugador(aj, golesVisitante);
        p.getActaVisitante().agregarConvocado(aj);
    }

    // Los goles restantes que no se asignaron a jugadores
    // se asignan al último convocado para cuadrar el marcador
    if (golesLocal > 0)
        p.getActaLocal().getConvocados()[10].setGoles(
            p.getActaLocal().getConvocados()[10].getGoles() + golesLocal);

    if (golesVisitante > 0)
        p.getActaVisitante().getConvocados()[10].setGoles(
            p.getActaVisitante().getConvocados()[10].getGoles() + golesVisitante);

    // Calcular goles totales reales de cada acta
    int totalLocal = 0, totalVisitante = 0;
    for (int i = 0; i < 11; i++) {
        totalLocal     += p.getActaLocal().getConvocados()[i].getGoles();
        totalVisitante += p.getActaVisitante().getConvocados()[i].getGoles();
    }

    p.getActaLocal().setGolesAFavor      (totalLocal);
    p.getActaLocal().setGolesEnContra    (totalVisitante);
    p.getActaVisitante().setGolesAFavor  (totalVisitante);
    p.getActaVisitante().setGolesEnContra(totalLocal);

    // Posesion
    double posLocal = calcularPosesion(local->getRankingFIFA(),
                                       visitante->getRankingFIFA());
    p.getActaLocal().setPosesion(posLocal);
    p.getActaVisitante().setPosesion(100.0 - posLocal);

    p.setResueltoEnProrroga(prorroga);
}

// Desempate por ranking — sesgo proporcional inverso al ranking
int Simulador::romperEmpate(const Equipo& local, const Equipo& visitante) const {
    double pesoLocal    = 1.0 / local.getRankingFIFA();
    double pesoVisitante = 1.0 / visitante.getRankingFIFA();
    double probLocal    = pesoLocal / (pesoLocal + pesoVisitante);
    return (randProb() < probLocal) ? 0 : 1;  // 0=local, 1=visitante
}

std::ostream& operator<<(std::ostream& os, const Simulador& s) {
    os << "Simulador [alpha=" << s.alpha
       << " beta=" << s.beta
       << " mu=" << s.mu << "]";
    return os;
}
