//#pragma once

// Local
#include "PrPixelHit.h"

#include <iostream>

namespace LHCb {
class State;
}

/** @class PrPixelTrack PrPixelTrack.h
 *  Working tracks, used inside the PrPixelTracking
 *
 *  @author Olivier Callot
 *  @date   2012-01-06
 */

class PrPixelTrack {

 public:
  /// Standard constructor
  PrPixelTrack(){;};
  // Creates a track from indexed data
  PrPixelTrack(std::vector<PrPixelHit*> && hits)
    : m_hits(hits), m_tx(0.0f), m_ty(0.0f), m_x0(0.0f), m_y0(0.0f) {
    }

  /// Destructor
  virtual ~PrPixelTrack() {}
  /// Start a seed track from the two given hits
  void set(PrPixelHit *h1, PrPixelHit *h2) {
    m_hits.clear();
    m_hits.push_back(h1);
    m_hits.push_back(h2);
    m_tx = (h1 -> x() - h2 -> x())/(h1 -> z() - h2 -> z());
    m_ty = (h1 -> y() - h2 -> y())/(h1 -> z() - h2 -> z());
    m_y0 = h1 -> y() - m_ty * h1 -> z();
    m_x0 = h1 -> x() - m_tx * h1 -> z();
    //std::cout << m_tx << " " << m_ty << std::endl;
  }

  /// Return the list of hits on this track.
  PrPixelHits & hits() { return m_hits; }
  /// Add a given hit to this track
  void addHit(PrPixelHit *hit) { m_hits.push_back(hit); }

  // Mark the hits of this track as being associated
  void tagUsedHits() {
    for (auto it = m_hits.begin(), end = m_hits.end(); it != end; ++it) {
      (*it)->setUsed(true);
    }
  }
  // Count unassociated hits for this track
  unsigned int nbUnused() const {
    unsigned int nn = 0;
    for (auto it = m_hits.cbegin(), end = m_hits.cend(); it != end; ++it) {
      if (!(*it)->isUsed()) ++nn;
    }
    return nn;
  }

  /// Chi2 / degrees-of-freedom of straight-line fit
  float chi2() const {
    float ch = 0.;
    int nDoF = -4;
    for (auto it = m_hits.cbegin(), end = m_hits.cend(); it != end; ++it) {
      ch += chi2(*it);
      nDoF += 2;
    }
    return ch / nDoF;
  }
  /// Chi2 constribution from a given hit
  float chi2(PrPixelHit *hit) const {
    return hit->chi2(m_x0 + m_tx * hit->z(), m_y0 + m_ty * hit->z());
  }
  /// Position at given z from straight-line fit
  float xAtZ(const float z) const { return m_x0 + m_tx * z; }
  float yAtZ(const float z) const { return m_y0 + m_ty * z; }

  // Calculate the z-pos. where the track passes closest to the beam
  float zBeam() const {
    return -(m_x0 * m_tx + m_y0 * m_ty) / (m_tx * m_tx + m_ty * m_ty);
  }

  //print the vector of hits
  void printPrPixelTrack() {
	int aux;
	std::cout << "[ ";	
	for(aux = 0; aux < size(); aux++){
		
		m_hits[aux] -> printPrPixelHit();
	}
	std::cout << " ]";
    }

  // Number of hits assigned to the track
  unsigned int size(void) const { return m_hits.size(); }

  //Get the value of Theta
  float Theta(){
    float x=xAtZ(100.);
    float y=yAtZ(100.);
    return sqrt(x*x+y*y)/100.;
  }

  //Get the value of Phi
  float Phi(){
    float x=xAtZ(100.);
    float y=yAtZ(100.);
    return acos(x/sqrt(x*x + y*y));
  }

  //Get the value of Eta
  float Eta(){
    float x=xAtZ(100.);
    float y=yAtZ(100.);
    return -log(Theta()/2);
  }	
 private:
  /// List of pointers to hits
  PrPixelHits m_hits;
  /// Straight-line fit parameters
  float m_tx;
  float m_ty;
  float m_x0;
  float m_y0;

};

typedef std::vector<PrPixelTrack> PrPixelTracks;  // vector of tracks
