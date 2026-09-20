# Design Parking Lot (Low-Level Design Focus)

A classic **LLD (Low-Level Design)** interview problem. Tests OO design, design patterns, and code clarity. Often asked as a 45-60 minute pair-programming round.

---

## 1. Requirements

### Functional
- Multiple floors, each with many spots.
- Different spot sizes: motorcycle, compact, large.
- Multiple vehicle types.
- Entry: assign spot, issue ticket.
- Exit: compute fee, free spot.
- Display board: count of free spots per type per floor.

### Non-Functional (LLD)
- Extensible for new vehicle / spot types.
- Thread-safe (multiple gates, simultaneous entry/exit).
- Clear responsibilities; use design patterns.

---

## 2. Core Classes

```
Vehicle (abstract)
├── Motorcycle
├── Car
└── Truck

Spot (abstract)
├── MotorbikeSpot
├── CompactSpot
└── LargeSpot

Floor
  - id
  - spots: List<Spot>

ParkingLot (singleton)
  - floors: List<Floor>
  - activeTickets: Map<TicketId, Ticket>

Ticket
  - id, vehicle, spot, entryTime

DisplayBoard
  - per-floor free counts

Gate / Entrance / Exit
```

---

## 3. Key Methods

```python
class ParkingLot:
 def find_spot(self, vehicle) -> Spot:
 for floor in self.floors:
 spot = floor.find_available(vehicle)
 if spot:
 return spot
 return None

 def park(self, vehicle) -> Ticket:
 spot = self.find_spot(vehicle)
 if not spot: raise FullException
 spot.assign(vehicle)
 ticket = Ticket(vehicle, spot, now())
 self.active_tickets[ticket.id] = ticket
 return ticket

 def unpark(self, ticket_id) -> Fee:
 ticket = self.active_tickets.pop(ticket_id)
 ticket.spot.release()
 return FeeCalculator.compute(ticket)
```

---

## 4. Spot Matching

A vehicle can only use spots of equal or larger size:
```
Motorcycle → MotorbikeSpot, CompactSpot, LargeSpot
Car → CompactSpot, LargeSpot
Truck → LargeSpot only
```

A spot can hold at most one vehicle at a time.

---

## 5. Fee Strategy

Different strategies by vehicle type, duration, customer loyalty.

```python
class FeeStrategy(ABC):
 def compute(self, ticket) -> Money: ...

class HourlyFee(FeeStrategy):
 RATES = { 'motorcycle': 10, 'car': 20, 'truck': 30 } # per hour
 def compute(self, t):
 hours = ceil((now - t.entry_time).hours)
 return self.RATES[t.vehicle.type] * hours

class FlatWithFirstHourFree(HourlyFee):
 def compute(self, t):
 base = super().compute(t) - self.RATES[t.vehicle.type]
 return max(base, 0)
```

Picked by strategy pattern → easy to add flat-rate, weekend, etc.

---

## 6. Design Patterns Used

| Pattern | Where |
|---|---|
| **Singleton** | `ParkingLot` |
| **Factory** | `VehicleFactory.create(type)`, `SpotFactory.create(type)` |
| **Strategy** | `FeeStrategy` |
| **State** | `Spot.availability` (free, occupied, reserved) |
| **Observer** | `DisplayBoard` listens to spot changes |
| **Facade** | `ParkingLot` API hides floor / spot details |

---

## 7. Concurrency

- Gates run on different threads.
- Spot assignment / release must be atomic.

Options:
- Per-spot lock (`Spot.lock`) — fine-grained.
- Synchronize on the `Floor` while picking a spot.

```python
class Floor:
 def find_available(self, vehicle):
 with self.lock:
 for spot in self.spots:
 if spot.can_fit(vehicle) and spot.is_free():
 return spot
 return None
```

Avoid deadlocks by always acquiring locks in a fixed order (e.g., ascending floor id).

---

## 8. Concurrency Models

- **In-process locks:** simple, fast, single-process parking lot.
- **DB row locks:** multi-process; per-spot row.
- **Redis atomic op / Lua:** for distributed gates.

---

## 9. Database Schema (if asked for HLD version)

```
spots: spot_id, floor, size, status, current_ticket_id
tickets: ticket_id, vehicle_no, spot_id, entry_time, exit_time, fee
vehicles: vehicle_no, type, owner_id
payments: payment_id, ticket_id, amount, method, time
```

---

## 10. Extensions (commonly asked)

- **Reservation:** mark spots as reserved for upcoming entry.
- **Electric vehicle charging:** only certain spots; add charging session tracking.
- **Multi-entry / multi-exit:** centralized spot manager with locks.
- **Dynamic pricing:** fee varies with demand (surge).
- **Lost ticket:** charge max daily rate.
- **Membership:** monthly pass users skip per-hour fee.
- **License-plate recognition:** auto-entry/exit.

---

## 11. Sample UML-ish Sketch

```
+------------------+ +------------------+
| ParkingLot |◄────| DisplayBoard |
| -floors | +------------------+
| -active_tickets| ▲
+--------+---------+ |
 | uses |
 ▼ |
+--------+---------+ +------+------+
| Floor | | FeeStrategy|
| -spots | | (interface) |
+----+--------+----+ +------+------+
 | |
 | owns |
 ▼ ▲
+----+--------+----+ implements +------+------+
| Spot (abstract) | | HourlyFee |
+----+--------+----+ +------+------+
 ▲
 |
+--------+ +----+-----+ +-----+----+
|Motorbike| |Compact | |Large |
+--------+ +---------+ +---------+
```

---

## 12. Follow-up Questions

**Q: How would you handle a spot that's also for EVs (charging)?**
Add a `Charger` attribute to the Spot; track charging session in the Ticket.

**Q: How to find the closest available spot?**
By walking distance: precompute per-spot (floor, distance from elevator). Pick the minimum.

**Q: How to make the design thread-safe?**
Locks on spot or floor. Avoid deadlocks with consistent ordering.

**Q: How to add a new vehicle type (e.g., bicycle)?**
Extend `Vehicle` (or `VehicleType` enum). Update spot-fit rules. No change to parking logic.

**Q: How to test?**
Unit-test each strategy / factory. Concurrency test: 100 gates try to park 1000 vehicles, no over-allocation.

**Q: How to scale to a multi-city chain?**
Each lot is independent. Centralized analytics for revenue, occupancy.

---

## 13. Final Sample Walk-through

```
Car "KA-01-1234" arrives at gate.
 Gate calls ParkingLot.park(car)
 ParkingLot scans floors, picks CompactSpot #B2-15.
 Issues Ticket #T-1001, entry = 09:00.
 Display board decrements Compact free count.

Customer returns at 11:30.
 Gate scans ticket.
 ParkingLot.unpark(T-1001) → 2.5 hours × 20 = 50 rupees.
 Spot released.
 Display board increments.
```

---

## 14. Take-aways

- Always start with **classes and relationships** — interviewers want to see your OO thinking.
- Use **design patterns** (Strategy, Factory, Singleton) to keep code extensible.
- Discuss **concurrency** explicitly.
- Mention **extensions** to show you've thought ahead.
