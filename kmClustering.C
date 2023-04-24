#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

// Function to generate random data from a mixed model
vector<double> genData(int nmEvents, int nmClusters) {
         
	 vector<double> data(nmEvents);
         random_device rd;
         mt19937 gen(rd());
         uniform_int_distribution<> dis(0, nmClusters - 1);

         for (int i = 0; i < nmEvents; ++i) {

                  if (dis(gen) == 0) {

                          //normal distribution with mean 0 and sig1

                        normal_distribution<> nd(-1.3, 0.2);
                        data[i] = nd(gen);
		  } else {
			  // uniform distribution between 0 and 1
                          uniform_real_distribution<> ud(1.3, 0.3);
                          data[i] = ud(gen);
		  }
            }
	 return data;
   }

// Function to implement the k-means clustering algorithm
vector<int> kmeans(vector<double>& data, int k, int nmIters) {
	vector<int> assignments(data.size());
        vector<double> means(k);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, data.size() - 1);
          // Initialize means to random data points
	  for (int i = 0; i < k; ++i) {
		  means[i] = data[dis(gen)];
	  }

	  for (int iter = 0; iter < nmIters; ++iter) {
		  // Assign each data point to the nearest mean
		  for (int i = 0; i < data.size(); ++i) {
			  double min_distance = numeric_limits<double>::max();
			  for (int j = 0; j < k; ++j) {
                
				  double distance = std::abs(data[i] - means[j]);
                
				  if (distance < min_distance) {
                   
					  min_distance = distance;
                   
					  assignments[i] = j;
                                 }
            
			  }
        
		  }
		  // Update means to the centroid of the assigned data points
        
		  for (int j = 0; j < k; ++j) {
            
			  double sum = 0.0;
            
			  int count = 0;
            
			  for (int i = 0; i < data.size(); ++i) {
               
				  if (assignments[i] == j) {
                   
					  sum += data[i];
                    
					  ++count;     
				  }
           		  }
            
			  if (count > 0) {
               
				  means[j] = sum / count;
            
			  }
       
		  }
    
	  }
    return assignments;
}

int kmClustering() {
      // Generate random data from a mixed model
    const  int mxEvents = 100;
    const  int mxClusters = 2;
      vector<double> data = genData(mxEvents, mxClusters);
      // Apply k-means clustering to the data
      int mxIters = 5;
      vector<int> assignments = kmeans(data, mxClusters, mxIters);
    
       auto hs = new THStack("hs","");
      auto *hd = new TH1F("hd", "Data Points from a Mixed Model", mxEvents, -2., 2.);
      auto *hm = new TH1F("hm", "k-means clustering implementation", mxEvents, -2., 2.);
      for (int i = 0; i < assignments.size(); ++i) {
	      hd->Fill(data[i]);
	      //cout<<data[i]<<endl;
              hm->Fill(assignments[i]);

           // cout << "   "<<assignments[i] << " ";//assignment yeilds
        }
      cout <<endl;
      hs->Add(hd);
     hs->Add(hm);
      hs->SetTitle("K-Means Clustering; data points; events");
      auto *c = new TCanvas();
      hd->SetMarkerStyle(20);
      hd->SetMarkerColor(3);
      hm->SetMarkerStyle(24);
      hm->SetMarkerColor(2);
      hd->GetXaxis()->SetTitle("data points");
      hs->Draw("AP");
      hs->GetXaxis()->SetTitle("data points");
      hs->GetYaxis()->SetTitle("Events");
     c->BuildLegend();
      c->Update();


    return 0;
}

