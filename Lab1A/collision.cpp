    /* 
    * AP Physics Lab 1A 
    * Collision Simulation
    * Jim Wu, Nelson Lee, Angela Pang, Eric Shang, Steven Zhang
    * SPH4U0
    *
    * Simulates the collision between two stress balls that involve a spring force
    * Needs to use the initial position and velocity of the two balls
    * Spring force is regressed and analyzed from experiment data
    * Will output the data into a .csv file
    * The file name can be changed inside the program below under: string file_name
    */

    // Use math constants defined by C++ (pi)
    #define _USE_MATH_DEFINES

    // define standard mass, radius, and time interval (dt)
    #define M 0.017
    #define R 0.0308
    #define DT 0.00001

    // libraries
    #include <iostream>
    #include <cmath>
    #include <string>
    #include <fstream>

    // use standard namespace
    using namespace std;

    class Vector { // Vector class
        public:
            // FIELDS

            double x, y, theta, m; // variables for rectangular and polar components
            bool rect; // rectangular or polar system

            // CONSTRUCTORS

            Vector() { // default vector with magnitude 0
                x = 0; // x
                y = 0; // y
                theta = 0; // angle
                m = 0; // magntiude
                rect = true; // rectangular system
            }

            Vector(double a, double b, bool rect = true) { // constructor for vectors with non-0 magnitude

                if (rect) { // rectangular (Cartesian) input
                    x = a; // x
                    y = b; // y

                    // create polar equivalent
                    m = sqrt(a * a + b * b); // magnitude = sqrt(x^2+y^2)
                    theta = atan2(b, a); // theta = tan^-1(y/x)
            
                }
                else { // polar input
                    m = a; // magnitude
                    theta = b; // angle

                    // create rectangular equivalent
                    x = m * cos(theta * M_PI / 180); // x = r cos(theta)
                    y = m * sin(theta * M_PI / 180); // y = r sin(theta)
                }        
                rect = true; // only deal with Cartesian if possible
            }

            // OTHER METHODS

            Vector operator - (Vector const &obj) { // overload operator to do vector subtraction
                // resultant vector (v) = vector - other vector
                Vector v = Vector(x-obj.x, y-obj.y, true); // components
                return v;
            }

            Vector negative() { // opposite vector
                return Vector(-x, -y, rect); // change sign of both components to be negative
            }    
    };

    // return Vector in a readable format of  x,y
    // overrides << (output) operator
    ostream& operator<<(ostream &strm, const Vector &v) {
        return strm << to_string(v.x) << "," << to_string(v.y); 
    }

    // Ball Class
    class Ball {
        public:
            // FIELDS

            // position, velocity, and acceleration vectors
            Vector s, v, a;
            // mass and radius
            double m, r;
            
            // CONSTRUCTOR
            
            Ball (Vector s0 = Vector(), Vector v0 = Vector(), double m0 = M, double r0 = R) { // constructor for ball
                // assign values from user or default (0 vectors and mass/radius constants)
                s = s0;
                v = v0;
                m = m0;
                r = r0;
            }

            // SPRING FORCES
            // Uses compression x as an argument
            // regression done using experiment data
            // since at x = 0 there should be no spring force, there is a 0 y-intercept in all the functions

            // linear regression -- not used
            double springLin(double x) {
                return 1045.02827 * x; // function is F = 1045.02827x
            }

            // quadratic regression -- not used
            double springQuad(double x) {
                return 11727.5 * x * x + 718.4 * x; // function is F = 11727x^2 + 718.4x
            }

            // x^(3/2) regression -- chosen one
            double spring32(double x) {
                return 6278 * pow(x, 3.0/2.0); // function is F = 6278x^(3/2)
            }

            // function that gets called
            double spring(double x) { // force that will be applied
                return spring32(x); // use the x^(3/2) regression
            }

            // OTHER METHODS

            // apply any force that is impart onto the ball
            void applyForce(Vector F, double dt) {

                /*
                * We know that F = ma, so a = F/m
                * Do this by components for x and y
                */
                a = Vector(F.x/m, F.y/m, true);

                // update new kinematic information
                // change is caused by a differnet acceration, so use kinematic equations where acceleration is constant (it is for this small dt)

                // standard kinematic equation: s = s0 + v0 * t + (1/2) * a * t^2 for both components
                double sx = s.x + v.x * dt + 0.5 * a.x * dt * dt;
                double sy = s.y + v.y * dt + 0.5 * a.y * dt * dt;

                // standard kinematic equation: v = v0 + a * t for both components
                double vx = v.x + a.x * dt;
                double vy = v.y + a.y * dt; 

                // update the new position and velocity with ones that are just calculated
                s = Vector(sx, sy, true);
                v = Vector(vx, vy, true);
            }
    };

    // this class holds the timing that tells the balls to move and eventually collide with each other
    class collision { // collision class
        public: 
            // static variables
            
            static Ball b1, b2; // balls
            static double dt, tlimit; // t intervals (dt) and limit for time

            // FUNCTIONS

            void getInput() { // helper function that prompts user to input kinematic (and some other information) for the balls

                // helper variables to take in input
                char coor = 'c', choice = 'n';
                // rectangular or polar input
                bool rect = true;

                // main first message
                cout << "\n--------Input your data:--------\n\n";

                // declare masses and radii
                double m1 = M, m2 = M, r1 = R, r2 = R;

                // MASSES

                // check if user wants to enter different variables for mass 
                cout << "Are the masses different than in the program? (y/n) ";
                cin >> choice; 

                if (choice == 'y') { // user wants to use different ones
                    // take in new masses
                    cout << "Mass of ball 1: ";
                    cin >> m1;
                    cout << "Mass of ball 2: ";
                    cin >> m2;
                }

                // RADII

                // check if user wants to enter different variables for radii 
                cout << "Are the radii different than in the program? (y/n) ";
                cin >> choice; 

                if (choice == 'y') { // user wants to use different ones
                    // take in new radii
                    cout << "Radius of ball 1: ";
                    cin >> r1;
                    cout << "Radius of ball 2: ";
                    cin >> r2;
                }

                // COORDINATE SYSTEM

                // find coordinate system (polar or cartesian) that the user wants to use
                cout << "Polar (p) or Cartesian (c)? ";
                cin >> coor; // take in input for coordinate system
                
                rect = (coor == 'c'); // convert polar/cartesian to a boolean value now
                
                string x_in, y_in; // placeholder variables to determine which outputs will be displayed
                
                // use these outputs in the prompt
                if (rect) { // rectangular
                    x_in = "X component of ";
                    y_in = "Y component of ";
                }
                
                else { // polar
                    x_in = "Magnitude of ";
                    y_in = "Angle (degrees) of ";
                }

                double in1 = 0, in2 = 0; // placeholder variables to take in the x and y (or mag and theta) of a vector

                // VECTOR INFORMATION

                // POSITION
                Vector s1, s2;

                // get x, y / mag, theta for s of ball 1
                cout << "Enter the " << x_in << "ball 1's position: ";
                cin >> in1; // input x or mag
                cout << "Enter the " << y_in << "ball 1's position: ";
                cin >> in2; // input y or theta

                s1 = Vector(in1, in2, rect); // create vector

                // get x, y or mag, theta for s of ball 2
                cout << "Enter the " << x_in << "ball 2's position: ";
                cin >> in1; // input x or mag
                cout << "Enter the " << y_in << "ball 2's position: ";
                cin >> in2; // input y or theta

                s2 = Vector(in1, in2, rect); // create vector

                // VELOCITY
                Vector v1, v2;

                // get x, y or mag, theta for v of ball 1
                cout << "Enter the " << x_in << "ball 1's velocity: ";
                cin >> in1; // input x or mag
                cout << "Enter the " << y_in << "ball 1's velocity: ";
                cin >> in2; // input y or theta

                v1 = Vector(in1, in2, rect); // create vector

                // get x, y / mag, theta for v of ball 2
                cout << "Enter the " << x_in << "ball 2's velocity: ";
                cin >> in1; // input x or mag
                cout << "Enter the " << y_in << "ball 2's velocity: ";
                cin >> in2; // input y or theta
    
                v2 = Vector(in1, in2, rect); // create vector

                // TIME LIMIT

                // get time limit for sim to run from user
                cout << "Enter the time limit: ";
                cin >> tlimit;

                // create the ball objects
                b1 = Ball(s1, v1, m1, r1);
                b2 = Ball(s2, v2, m2, r2);
            }
            
            int main() { // driving main function

                // assign the time increment to the constant defined above
                dt = DT;

                // setup file to write to
                string file_name = "out32.csv"; // file name
                ofstream f(file_name); // opens file with file_name

                Vector spring = Vector(); // creates spring force to be used later

                // headers
                // split into different lines to see better here
                string header = "Time (s),Ball 1 sx (m),Ball 1 sy (m),Ball 2 sx (m),Ball 2 sy (m),"; // time and position vectors
                header += "Ball 1 vx (ms^-1),Ball 1 vy (ms^-1),Ball 2 vx (ms^-1),Ball 2 vy (ms^-1),"; // velocity vectors
                header += "Ball 1 ax (ms^-2),Ball 1 ay (ms^-2),Ball 2 ax (ms^-2),Ball 2 ay (ms^-2),"; // acceleration vectors
                header += "Spring Force x (N),Spring Force y (N),\n"; // spring force
                
                f << header; // write header into file

                getInput(); // get input from user

                // loop for the simulation; starts at time being 0
                // uses dt as "steps" in time 
                // loops until it passes the time limit, tlimit
                for (double t = 0; t < tlimit; t += dt) {    

                    Vector distance = b1.s - b2.s; // find distance (difference) that the balls are away from each other by components

                    // write kinematic and force information into file
                    f << to_string(t) << "," << b1.s << "," << b2.s << "," << b1.v << "," << b2.v << "," << b1.a << "," << b2.a << "," << spring << ","<< distance.theta <<"\n";

                    /*find the "compression" distance
                    * this distance is the sum of the radii minus the distance they are apart
                    * if this distance is > 0, then they are touching because the radii combined is greater than their centre's distance apart
                    *   this should only happen when the balls start deformming --> spring force is applied
                    * if this distance is <= 0, they are not touching (the radii are smaller than the distasnce)
                    *   no spring force, so no additional accleration
                    */
                    double d_comp = b1.r + b2.r - distance.m; // distance_compressed

                    if (d_comp > 0) { // balls are touching, so there is a spring force applied
                        double spring_m = b1.spring(d_comp); // calculate the spring force magnitude using compression distance we've calculated

                        // distance.theta is already in radians
                        // convert spring force from polar to Cartesian using x = r cos(theta) and y = r sin(theta)
                        spring.x = spring_m * cos(distance.theta);
                        spring.y = spring_m * sin(distance.theta);
                    }
                    else { // balls are not touching, so there is no spring force (spring force is 0 since default Vector() is 0)
                        spring = Vector();
                    }

                    /* apply the force (0 if not touching, or the generated force if it is touching) to both balls
                    * this changes the kinematic properties of the ball (s, v, a), which is a step in the simulation
                    * force on both balls are equal in magnitude
                    */
                    b1.applyForce(spring, dt); // spring force is calculated above on b1
                    b2.applyForce(spring.negative(), dt); // N3, force that is equal and opposite
                }

                f.close(); // safely close the file written on
                cout << "done"; // tell user the program is done
                return 0;
            }        
    };

    // define static variables for use inside the class
    double collision::dt; // time interval
    double collision::tlimit; // simulation run time that user wil say
    Ball collision::b1; // ball 1
    Ball collision::b2; // ball 2

    // actual driver function
    int main() { // main function
        collision c = collision(); // create instance of collisions
        c.main(); // run collision
    }
