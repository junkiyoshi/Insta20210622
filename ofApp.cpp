#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->font_size = 25;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);

	this->word_list = {
	"A", "B", "C", "D", "E", "F",
	"G", "H", "I", "J", "K", "L",
	"M", "N", "O", "P", "Q", "R",
	"S", "T", "U", "V", "V", "X",
	"Y", "Z" };

	int sample_count = 39;

	for (auto& word : this->word_list) {

		vector<vector<glm::vec3>> vertices_list;
		vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);

		for (int word_index = 0; word_index < word_path.size(); word_index++) {

			vector<ofPolyline> outline = word_path[word_index].getOutline();

			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
				vertices_list.push_back(vertices);
			}
		}

		this->word_vertices_list.push_back(vertices_list);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofColor color;
	for (int i = 0; i < 256 * 3; i++) {

		auto word_index = i % this->word_list.size();
		auto noise_param = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		auto word_size = this->font.getStringBoundingBox(this->word_list[word_index], 0, 0);
		auto radius = ofRandom(90, 110);
		color.setHsb(i % 256, 180, 255);

		for (int k = 0; k < 2; k++) {

			if (k == 0) {

				ofFill();
				ofSetColor(color);
			}
			else {

				ofNoFill();
				ofSetColor(39);
			}

			ofBeginShape();
			for (int vertices_index = 0; vertices_index < this->word_vertices_list[word_index].size(); vertices_index++) {

				if (vertices_index != 0) { ofNextContour(true); }

				auto vertices = this->word_vertices_list[word_index][vertices_index];
				for (auto& vertex : vertices) {

					auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_param.x, vertex.x * 0.0001, ofGetFrameNum() * 0.0015), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
					auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_param.y, vertex.y * 0.0001, ofGetFrameNum() * 0.0015), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
					auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_param.z, vertex.z * 0.0001, ofGetFrameNum() * 0.0015), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 0, 1));

					glm::vec3 location = glm::vec4(glm::vec3(vertex - glm::vec3(word_size.getWidth() * 0.5, -word_size.getHeight() * 0.5, radius)), 0) * rotation_z * rotation_y *  rotation_x;
					ofVertex(location);
				}
			}
			ofEndShape(true);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}