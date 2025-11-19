create DATABASE SensorTemperatura;
use SensorTemperatura;

create table valores (
id INT auto_increment primary key, 
valor VARCHAR(50),
data_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    
);

select * from valores; 