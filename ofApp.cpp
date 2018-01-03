#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {
	for (int i = this->particles.size() - 1; i > -1; i--) {
		for (int j = this->particles[i].size() - 1; j > -1; j--) {
			delete this->particles[i][j];
			this->particles[i].erase(this->particles[i].begin() + j);
		}
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofSetLineWidth(0.5);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 150, true, false, true);

	vector<string> words;
	words.push_back("CODE");
	words.push_back("+");
	words.push_back("LIFE");

	ofColor body_color;
	
	for (int word_index = 0; word_index < words.size(); word_index++) {
		ofVec2f word_size = ofVec2f(this->font.stringWidth(words[word_index]), -this->font.stringHeight(words[word_index]));

		vector<ofTTFCharacter> char_paths = this->font.getStringAsPoints(words[word_index]);
		vector<Particle*> tmp_particles;

		for (int path_index = 0; path_index < char_paths.size(); path_index++) {
			vector<ofPolyline> outline = char_paths[path_index].getOutline();
			body_color.setHsb(ofRandom(255), 255, 255);

			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {
				outline[outline_index] = outline[outline_index].getResampledBySpacing(5);

				tmp_particles.clear();
				tmp_particles.shrink_to_fit();

				vector<ofPoint> vertices = outline[outline_index].getVertices();
				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {
					ofVec2f location = vertices[vertices_index] - word_size / 2;
					location.y = location.y + 150 * (word_index - 1);
					tmp_particles.push_back(new Particle(location, body_color));
				}

				this->particles.push_back(tmp_particles);
			}
		}
	}

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	this->fbo.begin();
	ofClear(0);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	ofVec2f mouse_point = ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
	ofDrawCircle(mouse_point, 3);

	for (int i = 0; i < this->particles.size(); i++) {
		for (int j = 0; j < this->particles[i].size(); j++) {
			float distance = mouse_point.distance(this->particles[i][j]->get_location());

			if (distance < 50) {
				this->particles[i][j]->seek_r(mouse_point);
			}
			else {
				this->particles[i][j]->seek(this->particles[i][j]->get_start_location());
			}

			this->particles[i][j]->update();
		}
	}

	for (int i = 0; i < this->particles.size(); i++) {
		//ofBeginShape();
		for (int j = 0; j < this->particles[i].size(); j++) {
			this->particles[i][j]->draw();
			//ofVertex(this->particles[i][j]->get_location().x, this->particles[i][j]->get_location().y);
		}
		//ofEndShape(true);
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	this->fbo.draw(0, 0);
}

//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}