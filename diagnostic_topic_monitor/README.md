# ROB Topic Monitor

This is a generic topic monitor to check that topic publication rate is as expected, reporting status using diagnostics.

## Acknowledgments

Originally developed for TOP-100 "BAUTIRO".

## Technical Debt

This code was originally developed for ROS 2 "Galactic" and contains backports of some functionality that are no longer necessary for later ROS 2 versions.

Moreover, the header_topic_monitor is largely copy-pasted.

## Usage

The nodes and components use the ROS 2 life-cycle based, which means they won't just activate when run. See the `test` folder for examples of how to configure and start the nodes.

> TIP: If you don't mind the added dependency, you can use the [nav2_lifecycle_manager](https://docs.nav2.org/configuration/packages/configuring-lifecycle.html) with parameter `autostart` set to `true` and target the monitor nodes to autostart them.

> Example launch snippet:
```python
# Include this node in your LaunchConfiguration
autostart_node = LifecycleNode(
        package="nav2_lifecycle_manager",
        executable="lifecycle_manager",
        name="autostart_monitors",
        parameters=[
            {
              "autostart": True,
              "node_names": ["topic_frequency_monitor", "topic_age_monitor"]
            }
        ],
    )
```

> For this, you need to have the `nav2_lifecycle_manager` package available (binary or source code installation), and ideally add it as an `exec_depend` in your package.

### Nodes

There are two nodes which behave almost the same
  * topic_frequency_monitor -- this supports _any_ kind of topic and checks for frequency based on receive time
  * topic_age_monitor -- this supports only topics with a Header, and it checks that message age is within a given range

It is totally possible to run both of these at the same time _for the same topics_, to check for both frequency
and max message age at the same time.

Some of the parameters are common for both nodes:

#### Limiting which topics to monitor

By default, the following internal topic names will be ignored, as they are usually irrelevant:

* ^/rosout$
* .*/parameter_events
* ^/diagnostics$
* .*/transition_event

`monitor_configured_only`
 If false (the default), the topic monitor will monitor everything but the internal topics mentioned above.
 To instead only monitor those topics which have been explicitly configured, set this to "true"

#### Influencing the output

`diag_prefix`
 This string will be prefixed to the name of the diagnostic for use within the aggregator.

### `topic_frequency_monitor`

Without any parameters, this node (component also available) will subscribe to all normal topics in a system and minimally monitor that there is activity on them (defined as "at least one message per second"). For more specific configuration, the following parameters are available.

#### Frequency Monitoring

Usually, we want to make sure that the frequency of a topic is within certain bounds.

For example, to monitor that the frequency of the "talker" topic is about 25Hz and the frequency of the "cmd_vel" topic is about 10Hz, use the following three parameters and pass them lists in the same order. This means, the second element in "min_values" applies to the topic given by the second element of the "topics" parameter.

    topics: [ "talker", "cmd_vel" ]
    min_values: [ 23.0, 8.0 ]
    max_values: [ 26.5, 11.0 ]

Please note that the frequencies will be computed purely based on _arrival_ timing, the header is currently ignored.

### `topic_age_monitor`

Without any parameters, this node (component also available) will subscribe to all normal topics in a system and minimally monitor that there is activity on them (defined as "at least one message per second"). For more specific configuration, the following parameters are available.

#### Message Age Monitoring

Usually, we want to make sure that message is not much older than expected (allowing for some jitter due to network transmission),
but also not newer than expected (which would indicate issues with timestamp or system time between machines).

Since this relies on a sender-side timestamp, this monitor only works with message that include a Header.

NOTE: You can configure it for any topic and it _will_ interpret the data in there as a timestamp, but unless it
really is a header, the results will be rather random ;-)

Configuration is similar to the frequency monitor, but giving a range for delays instead:

    topics: [ "talker", "cmd_vel" ]
    min_values: [ 80, 8.0 ]
    max_values: [ 120, 11.0 ]


## Testing

To run all tests in this package:

```bash
colcon test --event-handlers console_direct+ --packages-select diagnostic_topic_monitor
```
