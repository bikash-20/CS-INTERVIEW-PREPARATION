# Design Parking Lot (LLD Focus)

This is a popular **Low-Level Design (LLD)** interview question. Focus is on classes, relationships, design patterns.

---

## Requirements

- Multiple floors, multiple spot types (compact, large, bike, electric).
- Park / unpark vehicles.
- Issue tickets, calculate fees.
- Display availability.

---

## Classes

```
ParkingLot
 - name, address
 - List<Floor> floors
 - Singleton

Floor
 - number
 - List<ParkingSpot> spots
 - DisplayBoard

ParkingSpot
 - id, type (enum)
 - isFree
 - Vehicle parkedVehicle
 - assignVehicle(Vehicle)
 - removeVehicle()

Vehicle (abstract)
 - licensePlate
 - VehicleType type
class Car : Vehicle
class Bike : Vehicle
class Truck : Vehicle

Ticket
 - id, entryTime, exitTime
 - Vehicle vehicle, ParkingSpot spot
 - computeFee()

PricingStrategy (interface)
 - calculateFee(Ticket)
class HourlyPricing : PricingStrategy
class FlatPricing : PricingStrategy

ParkingLotService
 - parkVehicle(Vehicle) -> Ticket
 - unparkVehicle(Ticket) -> Fee
```

---

## Key Design Decisions

### 1. Spot-Vehicle Compatibility
- Bike → bike spot
- Car → compact or large spot
- Truck → large spot
- Electric → electric spot (with charger)

### 2. Spot Assignment Strategy
- Find nearest spot of compatible type.
- Hashmap of `VehicleType → List<free spots>` for O(1) allocation.

### 3. Pricing Strategy
- Different rates per vehicle type or time-of-day.
- Use **Strategy pattern**.

### 4. Concurrency
- Multiple gates assigning spots — use locking or atomic operations on free lists.

### 5. Singleton vs Multi-instance
- One parking lot → Singleton.
- Could be multi-instance if there are multiple physical lots.

---

## Sample Code Sketch (C++)

```cpp
enum class VehicleType { BIKE, CAR, TRUCK, ELECTRIC };
enum class SpotType { SMALL, MEDIUM, LARGE, ELECTRIC };

class Vehicle {
public:
 string licensePlate;
 virtual VehicleType getType() const = 0;
};
class Car : public Vehicle {
 VehicleType getType() const override { return VehicleType::CAR; }
};

class ParkingSpot {
 string id;
 SpotType type;
 bool isFree = true;
public:
 bool canFit(VehicleType v) const {
 // mapping logic
 }
 void park(Vehicle* v) { /* ... */ isFree = false; }
 void unpark() { isFree = true; }
};

class Floor {
 vector<ParkingSpot> spots;
public:
 ParkingSpot* findSpot(VehicleType v) {
 for (auto& s : spots) if (s.isFree && s.canFit(v)) return &s;
 return nullptr;
 }
};

class ParkingLot {
 static ParkingLot* instance;
 vector<Floor> floors;
public:
 static ParkingLot* getInstance() {
 if (!instance) instance = new ParkingLot();
 return instance;
 }
 Ticket* parkVehicle(Vehicle* v) {
 for (auto& f : floors) {
 ParkingSpot* s = f.findSpot(v->getType());
 if (s) {
 s->park(v);
 return new Ticket(s, v);
 }
 }
 return nullptr; // full
 }
};
```

---

## Extending the System

1. **Multiple entrances/exits** — Each has a gate; central service dispatches.
2. **Reservation system** — Pre-book a spot.
3. **EV charging** — Spot type with `ChargingPort`.
4. **Valet parking** — Vehicle moves without owner; ticket has token.
5. **Analytics** — Track usage, revenue per day.

---

## Patterns Used

- **Singleton** — ParkingLot
- **Strategy** — PricingStrategy
- **Factory** — Vehicle creation
- **State** — Spot state (free, occupied, reserved, out-of-service)
- **Observer** — DisplayBoard updates on spot changes

---

## Follow-up Questions

**Q: How to handle multi-entry gates?**
Centralized ParkingLotService; gates call into it. Lock per floor to avoid double-booking.

**Q: How to scale to multiple physical lots?**
Multi-tenant singleton; or use location ID.

**Q: How to design for fast display update?**
Each spot emits events (Observer pattern) → DisplayBoard subscribes.

**Q: How to handle lost tickets?**
Charge max fee or verify via license plate image.
